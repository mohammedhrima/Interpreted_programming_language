import basic

while True:
    text = input("tts > ")
    result, error = basic.run('<stdin>', text)
    if error:
        print(error.as_string())
    else:
        print(result)