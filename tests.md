1- expected
    x = "hello word"
    x = 123
    x = -123
    x = 1.5
    x = [1 + 5,2,3]
    x = ["h","e","l","l","o","w","o","r","d"]
    x = [y , 1 , 2]
    x = [ [1,2,3], [1,2,3], ["a","b","c"] ]
    x = {stname:"user1",ndname:"user1",age:12 }
    x = true
    x = false

2- expected
    x = "hello word"
    y = x[0]     -> y = 'h'
    x[0] = 'a'   -> x = "aello word"
    x = [1,2,3]
    x[0] = 5 -> x = [5,2,3]

3- expected
    x = "hello" + " world" -> x = "hello world"
    x = 1 + 2 -> x = 3
    x = [1,2,3] + [4,5,6]  -> x = [1,2,3,4,5,6]
    x = 1 == 2
    x = "abc" == "abc"
    x = 1 > 2 ...

    x = 1
    y = 1
    x = x + y -> x = 2

all the flowing operator should be handled:
    '<'         : less than
    '>'         : more than
    '+'         : add
    '-'         : subtract
    '/'         : divide
    '%'         : modulo
    '==' , 'is' : check if two values are equal
    '!' , 'not' : check if not
    '!=' , 'is not' : check if two values are not equal
    '++'        : increment
    '--'        : decrement

4- expected
    x = [1+2, 2-1, 3] -> x = [3,1,3]

6- expected
    output("hello world")
    output("hello world", 123, "abc")
    x = 123
    y = 456
    output(x, y)
    output(x + y)

7- expected
    x = 2
    if x == 2:
        x = 3
        if x == 3:
            x = 4
            if x == 4:
                x = 5
            output("end 3rd statement")
        output("end 2nd statement")
    output("end 1st statement")

    if x == 2:
        x = 3
        if x == 1:
            x = 4
            if x == 4:
                x = 5
            output("end 3rd statement") // output nothing
        output("end 2nd statement") 
    output("end 1st statement")


func hello(x, y ):
    output("x: ",x,"\n")
    output("y: ",y,"\n")
    output("x + y: ", x + y,"\n")


hello(1,2)


after:
- for loop should return array
- input to be verifiey