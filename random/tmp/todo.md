result:

+ skip new lines , and refresh level to 0
+ increment level with tab
+ get if token
+ get condition , expect ':'
+ build token with (level of if + 1)
+ if token level < received level, put end token
+ else continue

+ in execute keep executing till tokens[tk_pos] = end_statement_



- arithmetic:
  '<' : less than
  '>' : more than
  '+' : add
  '-' : subtract
  '/' : divide
  '==' , 'is' : check if two values are equal
  '!' , 'not' : check if not
  '!=' , 'is not' : check if two values are not equal
  '++' : increment
  '--' : decrement

- printf:
  output("some text")

- if-statement:
  if something1:
  action1
  else if something2:
  action2
  else:
  action3

- while-loop:
  while something1:
  action1

- functions:
  fun function_name(parm1, param2):
  do something
  return something

- expect new line at the end of each line
- varibale name can contains number also
- build ide that support it
- add thread that check availablife temporary and permanent variables
- Nunabale variables
- array of arrays
- object
- garbage collector
