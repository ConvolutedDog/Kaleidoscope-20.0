
#include "Lexer.h"

//===----------------------------------------------------------------------===//
// Lexer
//===----------------------------------------------------------------------===//

namespace Lexer {

int getchar(FILE *file) {
  return fgetc(file);
}

} // namespace Lexer
