#ifndef _CAPI_KIKAIME_H_
#define _CAPI_KIKAIME_H_

#ifndef __cplusplus
#error this is in fact a C++ header
#endif

#include <stddef.h>

namespace latinime {
    class Dictionary;
    class DicTraverseSession;
    class ProximityInfo;
}

class Suggestions;

using latinime::Dictionary;
using latinime::DicTraverseSession;
using latinime::ProximityInfo;

struct key {
    unsigned x, y;
    unsigned width, height;
    int code;
};

struct proximity_info_config {
    unsigned keyboard_width, keyboard_height;
    unsigned grid_width, grid_height;
    key *keys;
    size_t nkeys;
};

struct point {
    unsigned x, y;
    unsigned t;
};

struct context {
    const char **prev_words;
    const bool *begin_marks;
    size_t nwords;
};

extern "C" {

ProximityInfo *proximity_info_create(const proximity_info_config *);
void proximity_info_destroy(ProximityInfo *);

Dictionary *dictionary_create(const char *);
void dictionary_close(Dictionary *);

Dictionary *dictionary_new();
bool dictionary_exist(Dictionary *dict, const char *word, int prob);
bool dictionary_add(Dictionary *dict, const char *word, int prob);
bool dictionary_remove(Dictionary *dict, const char *word);
bool dictionary_write(Dictionary *dict, const char *path);

DicTraverseSession *session_create(const Dictionary *);
void session_close(DicTraverseSession *);

Suggestions *dictionary_predict(const Dictionary *, const context *);
Suggestions *dictionary_suggest(
        DicTraverseSession *, const Dictionary *, ProximityInfo *,
        const context *, const point *, unsigned, unsigned);

const char *suggestion_results_get(Suggestions *, unsigned);
unsigned suggestion_results_get_count(Suggestions *);
void suggestion_results_free(Suggestions *);

}

#endif
