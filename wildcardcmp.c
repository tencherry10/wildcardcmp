
#include <stdlib.h>
#include "wildcardcmp.h"

int wildcardcmp(const char *pattern, const char *string) {
  return better_wildcardcmp(string, pattern, 1, '\0');
}

//This function compares text strings, one of which can have wildcards ('*').
int better_wildcardcmp (
  const char  *pTameText,       // A string without wildcards
  const char  *pWildText,       // A (potentially) corresponding string with wildcards
  int         bCaseSensitive,   // By default, match on 'X' vs 'x'
  char        cAltTerminator)   // For function names, for example, you can stop at the first '('
{
  int         bMatch = 1;
  const char  *pAfterLastWild = NULL; // The location after the last '*', if we've encountered one
  const char  *pAfterLastTame = NULL; // The location in the tame string, from which we started after last wildcard
  char        t, w;
  if(pTameText == NULL) return 0;
  if(pWildText == NULL) return 0;
  
  // Walk the text strings one character at a time.
  while (1) {
    t = *pTameText;
    w = *pWildText;

    // How do you match a unique text string?
    if (!t || t == cAltTerminator) {
      // Easy: unique up on it!
      if (!w || w == cAltTerminator) {
        break;                                   // "x" matches "x"
      } else if (w == '*') {
        pWildText++;
        continue;                           // "x*" matches "x" or "xy"
      } else if (pAfterLastTame) {
        if (!(*pAfterLastTame) || *pAfterLastTame == cAltTerminator) {
          bMatch = 0;
          break;
        }
        pTameText = pAfterLastTame++;
        pWildText = pAfterLastWild;
        continue;
      }

      bMatch = 0;
      break;                                           // "x" doesn't match "xy"
    } else {
      if (!bCaseSensitive) {
        // Lowercase the characters to be compared.
        if (t >= 'A' && t <= 'Z') {
          t += ('a' - 'A');
        }

        if (w >= 'A' && w <= 'Z') {
          w += ('a' - 'A');
        }
      }

      // How do you match a tame text string?
      if (t != w) {
        // The tame way: unique up on it!
        if (w == '*') {
          pAfterLastWild = ++pWildText;
          pAfterLastTame = pTameText;
          w = *pWildText;
          if (!w || w == cAltTerminator) {
            break;                           // "*" matches "x"
          }
          continue;                           // "*y" matches "xy"
        } else if (pAfterLastWild) {
          if (pAfterLastWild != pWildText) {
            pWildText = pAfterLastWild;
            w = *pWildText;
            
            if (!bCaseSensitive && w >= 'A' && w <= 'Z') {
              w += ('a' - 'A');
            }

            if (t == w) {
              pWildText++;
            }
          }
          pTameText++;
          continue;                           // "*sip*" matches "mississippi"
        } else {
          bMatch = 0;
          break;                                   // "x" doesn't match "y"
        }
      }
    }
    pTameText++;
    pWildText++;
  }
  return bMatch;
}
