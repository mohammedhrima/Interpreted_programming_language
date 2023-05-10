- I tried to build a mini programming language that has syntax similar to python with C
- And redo some exercices from 42pool and Libft project with it
- I've also borrow some attributes from javascript X'D
- You can check the tutorial for more details

## How to run

- run bash script to build the executable
```bash
  ./run.sh 
```
- then run a.out
```bash
  ./a.out path_to_your_file.txt
```

## Introduction
#### 1- hello world
- first hello word:
    ```python
    output("hello world")
    ```

- comments:
    ```c
        // this is line of comment
    ```

    ```python
        """
        this is 
        bloc of 
        comments X'D
        """
    ```

#### 2- data types:
- variable declaration and data types:
    - number:
    ```python
        num = 123
        output("num is ", num, "\n")
    ```
    - characters (one character or string):
   ```python
        str = "hello world"
        output("str is ", str, "\n")
    ``` 
    - array:
    ```python
        array = [1, 2, 3, 4, 5]
        output("array has value: ", array, "\n")
    ```
    - object:
    ```python
        obj = {
            stname: "mohammed",
            ndname:"hrima",
            age:25
          }

        output("obj is ", obj, "\n")

        // in case you want to access one of object attributes    
        output("obj has stname ", obj.stname, " and ndname ", obj.ndname, " and age ", obj.age, "\n") 
    ```
    ps: you can split data types with comma ','  in ouput function like print does in python

    - boolean:
    ```python
        boolean = true
        output("this is boolean with value ", boolean, "\n")
        boolean = false
        output("this is boolean with value ", boolean, "\n")
    ```
    

    - Wait there is something else you may need to check
    - All data types has an attribute "type" that return data type name as string
    ```python
        output("str has type ", str.type, "\n")
        output("num has type ", num.type, "\n")
        output("obj has type ", obj.type, "\n")
        output("array has type ", array.type, "\n")
        output("boolean has type ", boolean.type, "\n")
    ```

#### 3- take input:
- example:
    ```python
        stname = input("Enter first name: ")
        ndname = input("Enter second name: ")
    ```
- to output the current input: \
    method 1 :
    ```python
        output("Your full name is ", stname, " ", ndname, "\n")
    ```
    method 2:
    ```python
        fullname = stname + " " + ndname // you can use + operator to concatinate two strings
    
        output("Your full name is ", fullname, "\n")

        // here is another aproach
        output("Your full name is " + fullname + "\n")
    ```
    
- input does read input as string and assign it to left variable, in case you want to read a number,\
    you can use tonum attribute, and here is what to do it 

    ```python
        num = input("Enter your birthyear > ").tonum
        output("Your age is: ", 2023 - num, "\n")

        // here is anotehr aproach
        num1 = input("Enter your birthyear > ")
        output("Your age is: ",  2023 - num1.tonum, "\n")
    ```

## Iterations and attributes
#### 1- characters:
- Iteration:
    ```python
        str = "this is a string"
        output("str[0] is ", str[0], "\n")
    ```
- concatination:
    ```python
        str1 = "hello "
        str2 = "world"
        output("str1 + str2 is ", str1 + str2)
    ````
- indexof:
    ```python
        str = "abcdefghijklmno"
        output("index of de in str ", str.indexof("de"), "\n")
        output("index of fe in str ", str.indexof("fe"), "\n") // return -1 if doesn't exist
    ```
- count:
    ```python
        str = "abcdefgahijaklmno"
        output("there is ",str.count("a")," a in str", "\n")
        output("there is ",str.count("z")," z in str", "\n")
    ```
- split:
    ```python
        str = "abcdefgahijaklmno"
        array = str.split("a")
        output("array is: ", array, "\n")
    ````
- trim:
    ```python
        str = "abcdefgahijaklmnoabc"
        output("trim str by 'abc' ", str ,"\n")
        output("str after triming ", str.trim("abc") ,"\n")
    ```
- startswith: (return boolean value)
    ```python
        str = "abcdefgahijaklmnoabc"
        output(str.startswith("abc") ,"\n")
        output(str.startswith("abce") ,"\n")
    ```
- endswith: (return boolean value)
    ```python
        str = "abcdefgahijaklmno"
        output(str.endswith("mno") ,"\n")
        output(str.endswith("mnop") ,"\n")
    ```
- toupper:
    ```python
        str = "abcdefg"
        output("to upper: ", str.toup, "\n")
    ```
- tolower:
    ```python
        str = "RSTUVWX"
        output("to low: ", str.tolow, "\n\n")
    ```
- tonumber:
    ```python
        str = "123"
        output(str.tonum, "\n\n")
    ```
- isupper:
    ```python
        str = "ABC"
        output(str.isup, "\n\n")     
    ```
- islower:
    ```python
        str = "abc"
        output(str.islow, "\n\n")
    ```
- ischaracter:
    ```python
        str = "abc"
        output(str.ischar, "\n\n")
    ```
- isnum:
    ```python
        str = "123"
        output(str.isnum, "\n\n")
    ```
- len:
    ```python
        str = "abcdefgi"
        output(str.len, "\n\n")
    ```
- type:
    ```python
        str = "abcd"
        output(str.type, "\n\n")
    ```

#### 2- number:
- base:
    ```python
        n = 10
        output("n in base 10 is: ", n.base("0123456789"), "\n")
        output("n in base 16 is: ", n.base("0123456789ABCDEF"), "\n")
        output("n in base 2 is: ", n.base("01"), "\n")
    ```
- tocharacter:
    ```python
        output("n to characters ", n.tochar,"\n")
    ```
- type:
    ```python
        output(n.type, "\n")
    ```
#### 3- array:
- iteration:
    ```python
        array = ["h", "e", "l", "l", "o", "\n"]
        output("array[0] is ", array[0], "\n")
    ````
- concatination:
    ```python
        array1 = [1,2,3]
        array2 = [4,5,6]
        array3 = array1 + array2
        output("array1 + array2 is ", array3)
    ```
- indexof:
    ```python
        array = [11,22,33]
        output("index of 22 in array ", array.indexof(22), "\n")
    ```
- count:
    ```python
        array = [11,22,33, 44, 55, 11, 22, 33, 11]
        output("there is ",array.count(11)," 11 in array", "\n")
    ```
- len:
    ```python
        array = [11,22,33, 44, 55, 11, 22, 33, 11]
        output(array.len, "\n")
    ```
- type:
    ```python
        array = [11,22,33, 44, 55, 11, 22, 33, 11]
        output(array.type, "\n")
    ```

#### 4- boolean:
- type:
    ```python
        value = true
        value = false
        output(value.type, "\n")
    ```

#### 5- object:
- you can access object values by there keys:
    ```python
        obj = {
                name: "mohammed",
                age: 25
              }
        output(obj, "\n")
        output("obj has name: ", obj.name, " and age ", obj.age,"\n")
        output("obj name has len ", obj.name.len,"\n")
    ```

## Logic operator
#### 1- equality:
- to check if two strings are the stname
    ```python
        str1 = "abcdef"
        str2 = "abcdef"
    ```
- method 1:
    ```python
        res = str1 == str2
        output("1st method: check if str1 is same as str2: ", res, "\n")
    ```
- method 2:
    ```python
        res = str1 is str2
        output("2nd method: check if str1 is same as str2: ", res, "\n")
    ```
    ```python
        x = 1
        y = 12
        output("check if x is equal to y: ", x == y, "\n")
    ```
#### 2- comparision:
- examples:
    ```python
        res = 6 < 66
        output("check if 6 is less than 66: ", res, "\n")
    ```
    ```python
        x = 1
        y = 12
        output("check if x is equal to y: ", x < y, "\n")
    ```
#### 3- and or:
- examples:
    - and:
    ```python
        res = 0 < 1 and 1 < 2
        output("test 'and': ", res, "\n")
    ```
    ```python
        res = 10 < 1 && 1 < 2
        output("test '&&': ", res, "\n")
    ```
    - or:
    ```python
        res = 10 < 1 or 1 < 2
        output("test 'or': ", res, "\n")
    ```
    ```python
        res = 10 < 1 || 1 < 2
        output("test '||': ", res, "\n")
    ```

## Loops and statements:
#### 1- if statament:
- examples:
    ```python
        x = 10
        if x % 2 == 0:
            output("is odd\n")
        else:
            output("is even\n")
    ```
    ```python
        y = -10
        if y == 0:
            output("is zero\n")
        elif y > 0:
            output("is more than zero\n")
        else:
            output("is less than zero\n")
    ```
#### 2- while loop:
- examples:
    ```python
        x = 0
        while x < 10:
            output("increment x: ", x ,"\n")
            x += 1
    ```
    ```python
        y = 10
        while y > 0:
            output("decrement y: ", y, "\n")
            y -= 1
    ```
#### 3- for loop:
- examples:
    ```python
        array = [11,22,33,44,55]
        for x in range(0, array.len):
            output("array[",x, "] is ", array[x], "\n")
    ```
    - range function: return array of number 
    ```python
        range(0, 10) // return [0,1,2,3,4,6,7,8,9]
        range(10, 0) // return [10,9,8,7,6,5,4,3,2,1]
    ```
    - you can use 'in' keyword to iterate over array:
     ```python
        array = [11,22,33,44,55]
        for x in range(0, array.len):
            output("array[",x, "] is ", array[x], "\n")
    ```
    - to iterate over string with for loop
    ```python
        str = "abcdefghijklmnop"
        for z in range(str.len - 1, 0):
            output("str[",z,"] is ", str[z], "\n")
    ```

## Functions:
- examples:
    ```python
        func sayHi(str):
            output("hello ",str, "\n")

        sayHi("Mohammed")
    ```
    - return keyword:
    ```python
        func add(x,y):
            return x + y

        output("2 + 5 is ", add(2, 5), "\n")
    ```

- I do believe this mini programming language X'D isn't the greatest in the world,
- but I enjoyed working on it, It was a great challenge
