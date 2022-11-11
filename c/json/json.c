/* BNF -- https://github.com/JetBrains/Grammar-Kit/blob/master/testData/livePreview/Json.bnf
{
  tokens = [
    space='regexp:\s+'
    string = "regexp:\"[^\"]*\"|'[^']*'"
    number = "regexp:(\+|\-)?\p{Digit}*"
    id = "regexp:\p{Alpha}\w*"
    comma = ","
    colon = ":"
    brace1 = "{"
    brace2 = "}"
    brack1 = "["
    brack2 = "]"
  ]
  extends("array|object|json")=value
}

root ::= json
json ::= array | object  { hooks=[wsBinders="null, null"] }
value ::= string | number | json {name="value" hooks=[leftBinder="GREEDY_LEFT_BINDER"]}

array ::= '[' [!']' item (!']' ',' item) *] ']' {pin(".*")=1 extends=json}
private item ::= json {recoverWhile=recover}
object ::= '{' [!'}' prop (!'}' ',' prop) *] '}' {pin(".*")=1 extends=json}
prop ::= [] name ':' value {pin=1 recoverWhile=recover} // remove [] to make NAME mandatory
name ::= id | string {name="name" hooks=[rightBinder="GREEDY_RIGHT_BINDER"]}
private recover ::= !(',' | ']' | '}' | '[' | '{')
*/

char *jp = NULL;

#define SkipSpace(p) while (isspace(*p)) p++;

typedef void (*kvFunction)(char *key, char *value);

char *parseArray(char *text, kvFunction *f) {
    char *p = text;
    SkipSpace(p);
    if (*p != '[') return p;
    p++;
    while (*p != ']') parseJson(p, f);
    return p;
}

char *parseObject(char *text, kvFunction *f) {
    char *p = text;
    SkipSpace(p);
    if (*p != '{') return p;
    p++;
    p = parseStr(p, f); //
}

char *parseJson(char *text, kvFunction *f) {
    char *p = text;
    SkipSpace(p);
    if (*p == '[') { // array
        p = parseArray(p, f);
    } else if (*p == '{') {
        p = parseObject(p, f);
    }
    return p;
}
