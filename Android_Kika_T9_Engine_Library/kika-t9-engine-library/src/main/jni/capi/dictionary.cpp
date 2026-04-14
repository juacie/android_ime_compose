#include "kikaime.h"

#include "suggest/core/dictionary/dictionary.h"
#include "suggest/core/result/suggestion_results.h"
#include "suggest/core/session/dic_traverse_session.h"
#include "suggest/core/suggest_options.h"
#include "dictionary/header/header_read_write_utils.h"
#include "dictionary/interface/dictionary_header_structure_policy.h"
#include "dictionary/structure/dictionary_structure_with_buffer_policy_factory.h"
#include "dictionary/utils/file_utils.h"

using namespace latinime;

Dictionary *dictionary_create(const char *file) {
    int size = FileUtils::getFileSize(file);
    if (size < 0)
        return nullptr;

    bool updatable = FileUtils::existsDir(file);

    DictionaryStructureWithBufferPolicy::StructurePolicyPtr dictionaryStructureWithBufferPolicy(
            DictionaryStructureWithBufferPolicyFactory::newPolicyForExistingDictFile(
                    file, 0, size, updatable));

    if (!dictionaryStructureWithBufferPolicy)
        return nullptr;

    return new Dictionary(std::move(dictionaryStructureWithBufferPolicy));
}

void dictionary_close(Dictionary *dict) {
    delete dict;
}

int32_t utf8_decode_char(const char **pptr) {
    int32_t code = 0;
    const uint8_t *&ptr = *(const uint8_t **)pptr;
    uint8_t first = *ptr;
    if (first < 0x80) {
        code = first & 0x7f;
        ptr += 1;
    }
    else if (first < 0xc0) {
        // in the middle of a sequence
    }
    else if (first < 0xe0) {
        code = (first & 0x1f) << 6 | (ptr[1] & 0x3f);
        ptr += 2;
    }
    else if (first < 0xf0) {
        code = (first & 0x0f) << 12 | (ptr[1] & 0x3f) << 6 | (ptr[2] & 0x3f);
        ptr += 3;
    }
    else if (first < 0xf8) {
        code = (first & 0x07) << 18 | (ptr[1] & 0x3f) << 12 | (ptr[2] & 0x3f) << 6
               | (ptr[3] & 0x3f);
        ptr += 4;
    }
    else if (first < 0xfc) {
        code = (first & 0x03) << 24 | (ptr[1] & 0x3f) << 18 | (ptr[2] & 0x3f) << 12
               | (ptr[3] & 0x3f) << 6 | (ptr[4] & 0x3f);
        ptr += 5;
    }
    else if (first < 0xfe) {
        code = (first & 0x01) << 30 | (ptr[1] & 0x3f) << 24 | (ptr[2] & 0x3f) << 18
                | (ptr[3] & 0x3f) << 12 | (ptr[4] & 0x3f) << 6 | (ptr[5] & 0x3f);
        ptr += 6;
    }

    return code;
}

int utf8_decode_string(int *dst, unsigned cap, const char *src) {
    unsigned n = 0;
    while (*src && n < cap) {
        *dst++ = utf8_decode_char(&src);
        n++;
    }

    return n;
}

int utf8_encode_char(char *dst, unsigned cap, int32_t code) {
    unsigned di = 0;

    if (code < 0x7F) { // One byte
        dst[di++] = code;
    } else if (code < 0x7FF) { // Two bytes
        if (cap < 2) return 2;
        dst[di++] = 0xC0 + (code >> 6);
        dst[di++] = 0x80 + (code & 0x3F);
    } else if (code < 0xFFFF) { // Three bytes
        if (cap < 3) return 3;
        dst[di++] = 0xE0 + (code >> 12);
        dst[di++] = 0x80 + ((code >> 6) & 0x3F);
        dst[di++] = 0x80 + (code & 0x3F);
    } else if (code <= 0x1FFFFF) { // Four bytes
        if (cap < 4) return 4;
        dst[di++] = 0xF0 + (code >> 18);
        dst[di++] = 0x80 + ((code >> 12) & 0x3F);
        dst[di++] = 0x80 + ((code >> 6) & 0x3F);
        dst[di++] = 0x80 + (code & 0x3F);
    } else if (code <= 0x3FFFFFF) { // Five bytes
        if (cap < 5) return 5;
        dst[di++] = 0xF8 + (code >> 24);
        dst[di++] = 0x80 + ((code >> 18) & 0x3F);
        dst[di++] = 0x80 + ((code >> 12) & 0x3F);
        dst[di++] = 0x80 + ((code >> 6) & 0x3F);
        dst[di++] = code & 0x3F;
    } else if (code <= 0x7FFFFFFF) { // Six bytes
        if (cap < 6) return 6;
        dst[di++] = 0xFC + (code >> 30);
        dst[di++] = 0x80 + ((code >> 24) & 0x3F);
        dst[di++] = 0x80 + ((code >> 18) & 0x3F);
        dst[di++] = 0x80 + ((code >> 12) & 0x3F);
        dst[di++] = 0x80 + ((code >> 6) & 0x3F);
        dst[di++] = code & 0x3F;
    } else {
        // Not a code point... skip.
    }

    return di;
}

int utf8_encode_string(char *dst, unsigned cap, const int *src) {
    char *ptr = dst;

    while (*src) {
        int n = utf8_encode_char(ptr, cap, *src);
        ptr += n;
        cap -= n;
        src++;
    }

    if (cap)
        *ptr++ = '\0';

    return ptr - dst;
}

class Suggestions {
public:
    Suggestions(latinime::SuggestionResults *p)
            : internal(p) {
        get_all();
    }

    ~Suggestions() {
        delete internal;
    }

    unsigned size() const {
        return buffer.size();
    }

    const char *get(unsigned indx) const {
        if (indx >= size())
            return nullptr;

        return buffer[indx].first.c_str();
    }

    int getScore(unsigned indx) const {
        return buffer[indx].second;
    }

private:
    latinime::SuggestionResults *internal;

    typedef std::pair<std::string, int> _Pair;
    std::vector<_Pair> buffer;

    void get_all() {
        unsigned N = internal->getSuggestionCount();

        int codePoints[N * MAX_WORD_LENGTH];
        int scores[N];
        int spaces[N];
        int types[N];
        int confident;

        internal->outputSuggestions(codePoints, scores, spaces, types, &confident);

        for (unsigned i = 0; i < N; i++) {
            char tmp[6 * MAX_WORD_LENGTH];
            utf8_encode_string(tmp, sizeof(tmp), codePoints + i * MAX_WORD_LENGTH);
            buffer.emplace_back(tmp, scores[i]);
        }

        // larger score come first
        std::sort(buffer.begin(), buffer.end(),
                [](const _Pair &lhs, const _Pair &rhs) {
            return lhs.second > rhs.second;
        });
    }
};

const char *suggestion_results_get(Suggestions *results, unsigned indx) {
    return results->get(indx);
}

unsigned suggestion_results_get_count(Suggestions *results) {
    return results->size();
}

void suggestion_results_free(Suggestions *results) {
    delete results;
}

static NgramContext build_context(const context *ctx) {
    if (!ctx)
        return NgramContext();

    unsigned N = ctx->nwords;
    int codePoints[N][MAX_WORD_LENGTH];
    int lengths[N];
    bool startOfSentenceMarks[N];

    for (unsigned i = 0; i < N; i++) {
        lengths[i] = utf8_decode_string(codePoints[i], MAX_WORD_LENGTH, ctx->prev_words[i]);
        startOfSentenceMarks[i] = ctx->begin_marks ? ctx->begin_marks[i] : false;
    }

    return NgramContext(codePoints, lengths, startOfSentenceMarks, N);
}

Suggestions *dictionary_predict(const Dictionary *dict, const context *ctx) {
    NgramContext context = build_context(ctx);
    latinime::SuggestionResults *results = new latinime::SuggestionResults(MAX_RESULTS);

    dict->getPredictions(&context, results);
    return new Suggestions(results);
}

Suggestions *dictionary_suggest(
        DicTraverseSession *session, const Dictionary *dict, ProximityInfo *prox,
        const context *ctx, const point *points, unsigned n, unsigned weight) {\
    int raw_options[6] = { 0, 0, 1, 0, 1000 };
    raw_options[4] = weight;
    SuggestOptions options(raw_options, 5);
    NgramContext context = build_context(ctx);

    latinime::SuggestionResults *results = new latinime::SuggestionResults(MAX_RESULTS);

    int all_x[n];
    int all_y[n];
    int all_t[n];
    int all_p[n];

    for (unsigned i = 0; i < n; i++) {
        all_x[i] = points[i].x;
        all_y[i] = points[i].y;
        all_t[i] = points[i].t;
        all_p[i] = 0;
    }

    int dummy[n];
    memset(dummy, -1, sizeof(int) * n);

    session->getSuggestions(prox, dict, all_x, all_y, all_t, all_p,
                            dummy /* only used when typing */,
                         n, &context, &options, 1.0f, results);

    return new Suggestions(results);
}

Dictionary *dictionary_new() {
    DictionaryHeaderStructurePolicy::AttributeMap attributeMap;

    std::vector<int> key, value;
    HeaderReadWriteUtils::insertCharactersIntoVector("dictionary", &key);
    HeaderReadWriteUtils::insertCharactersIntoVector("<dict name>", &value);
    attributeMap[key] = value;

    key.clear();
    value.clear();
    HeaderReadWriteUtils::insertCharactersIntoVector("version", &key);
    HeaderReadWriteUtils::insertCharactersIntoVector("<dict ver.>", &value);
    attributeMap[key] = value;

    key.clear();
    value.clear();
    HeaderReadWriteUtils::insertCharactersIntoVector("date", &key);
    HeaderReadWriteUtils::insertCharactersIntoVector("<dict date>", &value);
    attributeMap[key] = value;

    value.clear();
    HeaderReadWriteUtils::insertCharactersIntoVector("en_US", &value);

    DictionaryStructureWithBufferPolicy::StructurePolicyPtr dictionaryStructureWithBufferPolicy =
            DictionaryStructureWithBufferPolicyFactory::newPolicyForOnMemoryDict(
                    FormatUtils::VERSION_403, value, &attributeMap);

    if (!dictionaryStructureWithBufferPolicy)
        return nullptr;

    return new Dictionary(std::move(dictionaryStructureWithBufferPolicy));
}

bool dictionary_add(Dictionary *dict, const char *word, int prob) {
    UnigramProperty unigramProperty(false, false, false, false, prob, HistoricalInfo());

    int codePoints[MAX_WORD_LENGTH];
    int len = utf8_decode_string(codePoints, MAX_WORD_LENGTH, word);

    return dict->addUnigramEntry(CodePointArrayView(codePoints, len), &unigramProperty);
}

bool dictionary_exist(Dictionary *dict, const char *word, int prob) {
    UnigramProperty unigramProperty(false, false, false, false, prob, HistoricalInfo());

    int codePoints[MAX_WORD_LENGTH];
    int len = utf8_decode_string(codePoints, MAX_WORD_LENGTH, word);

    int freq =dict->getProbability(CodePointArrayView(codePoints, len));

    return freq >0;
}

bool dictionary_remove(Dictionary *dict, const char *word) {
    int codePoints[MAX_WORD_LENGTH];
    int len = utf8_decode_string(codePoints, MAX_WORD_LENGTH, word);

    return dict->removeUnigramEntry(CodePointArrayView(codePoints, len));
}

bool dictionary_write(Dictionary *dict, const char *path) {
    return dict->flush(path);
}
