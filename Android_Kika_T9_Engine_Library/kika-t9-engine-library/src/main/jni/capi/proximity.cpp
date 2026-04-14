#include "kikaime.h"
#include "ArrayRef.h"
#include "suggest/core/layout/proximity_info.h"

#include <algorithm>
#include <unordered_map>

using namespace latinime;

const float SEARCH_DISTANCE = 1.2f;

static int squaredDistanceToEdge(const key &k, int x, int y) {
    const int left = k.x;
    const int right = left + k.width;
    const int top = k.y;
    const int bottom = top + k.height;
    const int edgeX = x < left ? left : (x > right ? right : x);
    const int edgeY = y < top ? top : (y > bottom ? bottom : y);
    const int dx = x - edgeX;
    const int dy = y - edgeY;
    return dx * dx + dy * dy;
}

// This function is identical to ProximityInfo::computeNearestNeighbors() in Java.
static int* compute_nearest_neighbors(const proximity_info_config &config, int common_key_width) {
    const int cellWidth = (config.keyboard_width + config.grid_width - 1) / config.grid_width;
    const int cellHeight = (config.keyboard_height + config.grid_height - 1) / config.grid_height;
    const int halfCellWidth = cellWidth / 2;
    const int halfCellHeight = cellHeight / 2;
    const int keyCount = config.nkeys;
    const int gridSize = config.grid_width * config.grid_height;
    const int threshold = (int)(common_key_width * SEARCH_DISTANCE);
    const int thresholdSquared = threshold * threshold;

    const int lastPixelXCoordinate = config.grid_width * cellWidth - 1;
    const int lastPixelYCoordinate = config.grid_height * cellHeight - 1;

    // For large layouts, 'neighborsFlatBuffer' is about 80k of memory: gridSize is usually 512,
    // keycount is about 40 and a pointer to a Key is 4 bytes. This contains, for each cell,
    // enough space for as many keys as there are on the keyboard. Hence, every
    // keycount'th element is the start of a new cell, and each of these virtual subarrays
    // start empty with keycount spaces available. This fills up gradually in the loop below.
    // Since in the practice each cell does not have a lot of neighbors, most of this space is
    // actually just empty padding in this fixed-size buffer.
    const key* neighborsFlatBuffer[gridSize * keyCount];
    int neighborCountPerCell[gridSize];
    memset(neighborCountPerCell, 0, sizeof(neighborCountPerCell));

/* HOW WE PRE-SELECT THE CELLS (iterate over only the relevant cells, instead of all of them)

  We want to compute the distance for keys that are in the cells that are close enough to the
  key border, as this method is performance-critical. These keys are represented with 'star'
  background on the diagram below. Let's consider the Y case first.

  We want to select the cells which center falls between the top of the key minus the threshold,
  and the bottom of the key plus the threshold.
  topPixelWithinThreshold is key.mY - threshold, and bottomPixelWithinThreshold is
  key.mY + key.mHeight + threshold.

  Then we need to compute the center of the top row that we need to evaluate, as we'll iterate
  from there.

(0,0)----> x
| .-------------------------------------------.
| |   |   |   |   |   |   |   |   |   |   |   |
| |---+---+---+---+---+---+---+---+---+---+---|   .- top of top cell (aligned on the grid)
| |   |   |   |   |   |   |   |   |   |   |   |   |
| |-----------+---+---+---+---+---+---+---+---|---'                          v
| |   |   |   |***|***|*_________________________ topPixelWithinThreshold    | yDeltaToGrid
| |---+---+---+-----^-+-|-+---+---+---+---+---|                              ^
| |   |   |   |***|*|*|*|*|***|***|   |   |   |           ______________________________________
v |---+---+--threshold--|-+---+---+---+---+---|          |
  |   |   |   |***|*|*|*|*|***|***|   |   |   |          | Starting from key.mY, we substract
y |---+---+---+---+-v-+-|-+---+---+---+---+---|          | thresholdBase and get the top pixel
  |   |   |   |***|**########------------------- key.mY  | within the threshold. We align that on
  |---+---+---+---+--#+---+-#-+---+---+---+---|          | the grid by computing the delta to the
  |   |   |   |***|**#|***|*#*|***|   |   |   |          | grid, and get the top of the top cell.
  |---+---+---+---+--#+---+-#-+---+---+---+---|          |
  |   |   |   |***|**########*|***|   |   |   |          | Adding half the cell height to the top
  |---+---+---+---+---+-|-+---+---+---+---+---|          | of the top cell, we get the middle of
  |   |   |   |***|***|*|*|***|***|   |   |   |          | the top cell (yMiddleOfTopCell).
  |---+---+---+---+---+-|-+---+---+---+---+---|          |
  |   |   |   |***|***|*|*|***|***|   |   |   |          |
  |---+---+---+---+---+-|________________________ yEnd   | Since we only want to add the key to
  |   |   |   |   |   |   | (bottomPixelWithinThreshold) | the proximity if it's close enough to
  |---+---+---+---+---+---+---+---+---+---+---|          | the center of the cell, we only need
  |   |   |   |   |   |   |   |   |   |   |   |          | to compute for these cells where
  '---'---'---'---'---'---'---'---'---'---'---'          | topPixelWithinThreshold is above the
                                        (positive x,y)   | center of the cell. This is the case
                                                         | when yDeltaToGrid is less than half
  [Zoomed in diagram]                                    | the height of the cell.
  +-------+-------+-------+-------+-------+              |
  |       |       |       |       |       |              | On the zoomed in diagram, on the right
  |       |       |       |       |       |              | the topPixelWithinThreshold (represented
  |       |       |       |       |       |      top of  | with an = sign) is below and we can skip
  +-------+-------+-------+--v----+-------+ .. top cell  | this cell, while on the left it's above
  |       | = topPixelWT  |  |  yDeltaToGrid             | and we need to compute for this cell.
  |..yStart.|.....|.......|..|....|.......|... y middle  | Thus, if yDeltaToGrid is more than half
  |   (left)|     |       |  ^ =  |       | of top cell  | the height of the cell, we start the
  +-------+-|-----+-------+----|--+-------+              | iteration one cell below the top cell,
  |       | |     |       |    |  |       |              | else we start it on the top cell. This
  |.......|.|.....|.......|....|..|.....yStart (right)   | is stored in yStart.

  Since we only want to go up to bottomPixelWithinThreshold, and we only iterate on the center
  of the keys, we can stop as soon as the y value exceeds bottomPixelThreshold, so we don't
  have to align this on the center of the key. Hence, we don't need a separate value for
  bottomPixelWithinThreshold and call this yEnd right away.
*/
    for (auto &&K : ArrayRef<key>(config.keys, config.nkeys)) {
        const int keyX = K.x;
        const int keyY = K.y;
        const int topPixelWithinThreshold = keyY - threshold;
        const int yDeltaToGrid = topPixelWithinThreshold % cellHeight;
        const int yMiddleOfTopCell = topPixelWithinThreshold - yDeltaToGrid + halfCellHeight;
        const int yStart = std::max(halfCellHeight,
                                    yMiddleOfTopCell + (yDeltaToGrid <= halfCellHeight ? 0 : cellHeight));
        const int yEnd = std::min(lastPixelYCoordinate, keyY + (int)K.height + threshold);

        const int leftPixelWithinThreshold = keyX - threshold;
        const int xDeltaToGrid = leftPixelWithinThreshold % cellWidth;
        const int xMiddleOfLeftCell = leftPixelWithinThreshold - xDeltaToGrid + halfCellWidth;
        const int xStart = std::max(halfCellWidth,
                                    xMiddleOfLeftCell + (xDeltaToGrid <= halfCellWidth ? 0 : cellWidth));
        const int xEnd = std::min(lastPixelXCoordinate, keyX + (int)K.width + threshold);

        int baseIndexOfCurrentRow = (yStart / cellHeight) * config.grid_width + (xStart / cellWidth);
        for (int centerY = yStart; centerY <= yEnd; centerY += cellHeight) {
            int index = baseIndexOfCurrentRow;
            for (int centerX = xStart; centerX <= xEnd; centerX += cellWidth) {
                if (squaredDistanceToEdge(K, centerX, centerY) < thresholdSquared) {
                    neighborsFlatBuffer[index * keyCount + neighborCountPerCell[index]] = &K;
                    ++neighborCountPerCell[index];
                }
                ++index;
            }
            baseIndexOfCurrentRow += config.grid_width;
        }
    }

    int *proximity_chars = new int[gridSize * MAX_PROXIMITY_CHARS_SIZE];

    for (int i = 0; i < gridSize; ++i) {
        const key **src = neighborsFlatBuffer + (i * keyCount);
        int *dst = proximity_chars + (i * MAX_PROXIMITY_CHARS_SIZE);
        int N = std::min(MAX_PROXIMITY_CHARS_SIZE, neighborCountPerCell[i]);

        for (int j = 0; j < N; j++) {
            dst[j] = src[j]->code;
        }

        for (int j = N; j < MAX_PROXIMITY_CHARS_SIZE; j++) {
            dst[j] = NOT_A_CODE_POINT;
        }
    }

    return proximity_chars;
}

ProximityInfo *proximity_info_create(const proximity_info_config *config) {
    // Calculate most common key width/height.
    std::unordered_map<int, int> key_widths, key_heights;

    for (unsigned i = 0; i < config->nkeys; i++) {
        auto &&key = config->keys[i];

        auto it = key_widths.find(key.width);
        if (it != key_widths.end()) {
            it->second++;
        }
        else {
            key_widths.emplace(key.width, 1);
        }

        it = key_heights.find(key.height);
        if (it != key_heights.end()) {
            it->second++;
        }
        else {
            key_heights.emplace(key.height, 1);
        }
    }

    typedef std::unordered_map<int, int>::value_type value_t;
    auto comp = [](value_t lhs, value_t rhs) {
        return lhs.second < rhs.second;
    };

    auto largest = std::max_element(key_widths.begin(), key_widths.end(), comp);
    int most_common_key_width = largest->first;

    largest = std::max_element(key_heights.begin(), key_heights.end(), comp);
    int most_common_key_height = largest->first;

    int x_cords[config->nkeys];
    int y_cords[config->nkeys];
    int widths[config->nkeys];
    int heights[config->nkeys];
    int codes[config->nkeys];

    int i = 0;
    for (auto &&k : ArrayRef<const key>(config->keys, config->nkeys)) {
        x_cords[i] = k.x;
        y_cords[i] = k.y;
        widths[i] = k.width;
        heights[i] = k.height;
        codes[i] = k.code;
        i++;
    }

    int *proximity_chars = compute_nearest_neighbors(*config, most_common_key_width);

    ProximityInfo *p = new ProximityInfo(config->keyboard_width, config->keyboard_height,
            config->grid_width, config->grid_height,
            most_common_key_width, most_common_key_height,
            proximity_chars, config->nkeys, x_cords, y_cords, widths, heights, codes,
            nullptr, nullptr, nullptr);

    delete[] proximity_chars;
    return p;
}

void proximity_info_destroy(ProximityInfo *proximity) {
    delete proximity;
}
