#include "kikaime.h"
#include "suggest/core/session/dic_traverse_session.h"

using namespace latinime;

DicTraverseSession *session_create(const Dictionary *dict) {
    return (DicTraverseSession*)DicTraverseSession::getSessionInstance(256 * 1024 + 1);
}

void session_close(DicTraverseSession *session) {
    DicTraverseSession::releaseSessionInstance(session);
}
