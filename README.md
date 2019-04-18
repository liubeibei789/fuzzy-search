# fuzzy-search
search a document for a word/phrase that may contain a typographical error.\
This is a second homework from my manager :) \
`Note:` **I am not sure the source of this problem...problem definition was from some others probably, but the solution was by myself.**
- The program should prompt the user for the name of a file to search and a phrase to search for. 
- The program should search for the original phrase in the contents of the file. 
  - If it finds the phrase, then it should display the first index at which it found the phrase as well as that it found an exact match. 
  - If the original phrase is not found, try searching for all one-letter deviations from the original phrase and display all of them that are found – both their index and the phrase in context. 
    - One letter deviations mean that one letter (and only one letter) of the original phrase was omitted, one letter was replaced with any other possible character, one letter swapped with an adjacent letter, or one letter was added at any position in the original phrase. 
    - Since you don’t have a fixed alphabet for your searches, you may want to consider how to represent the concept of a wildcard character in your code. Your code should display the index as well as the approximate match for all of the approximate matches that are found. 
    - After the list of approximate matches, your code should display how many approximate matches there were. 
- Library Usage: not permitted to use them for this assignment
 
## Approximate Matches: 
Depending on your search approach, you may find that you’re getting different numbers of matches. 
- My implementation leaves in newline characters – if you strip them out, you may find additional matches. 
- My implementation also removes duplicate matches – for example, if searching for “hello” and my code found an approximate match for “helo” by deleting the third letter, it wouldn’t also flag the same “helo” as a match for the search term with the fourth letter deleted.
### reference:
- Sample Approximate Match Outputs:\
Searching small.txt for “ware”:\
4 deletions: [5-7: are], [19-21: are], [35-37: are], [43-45: are]\
0 transpositions\
6 substitutions: [4-7: hare], [18-21: rare], [34-37: dare], [42-45: eare], [8-11: were], [12-15: wore]\
0 insertions\

- Searching small.txt for “xe”:\
22 deletions: [1: e], [3: e], [7: e], [9: e], [11: e], [15: e], [17: e], [21: e], [23: e], [29: e], [31: e], [37: e], [38: e], [42: e], [45: e], [48: e], [51: e], [54: e], [56: e], [58: e], [59: e], [62: e]\
0 transpositions\
22 substitutions: [0-1: he], [2-3: re], [6-7: re], [8-9: we], [10-11: re], [14-15: re], [16-17: he], [20-21: re], [22-23: te], [28-29: re], [30-31: de], [36-37: re], [37-38: ee], [41-42: re], [44-45: re], [47-48: re], [50-51: he], [53-54: se], [55-56: re], [57-58: se], [58-59: ee], [61-62: se]\
0 insertions\
 
