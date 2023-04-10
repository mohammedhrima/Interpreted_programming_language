import signal


def handler(sig, frame):
    exit(0)


INTEGER, PLUS, MINUS, MULTIPLICATION, DIVISION, EOF = (
    'int', '+', '-', '*', '/', 'EndOfFile')


class Token(object):
    def __init__(self, type, value):
        self.type = type
        self.value = value

    def __repr__(self):
        return "Token value is " + str(self.value) + " type is " + str(self.type)


class Lexer(object):
    def __init__(self, text):
        self.text = text
        self.position = 0

    def error(msg):
        raise Exception(msg)

    def skip_space(self):
        if self.text is None:
            self.error("text is None")
        while self.text[self.position].isspace():
            self.position += 1

    def get_number(self):
        self.skip_space()
        if not self.text:
            self.error("text is None")
        result = ''
        while self.position < len(self.text) and self.text[self.position].isdigit():
            result += self.text[self.position]
            self.position += 1
        self.skip_space()

        return int(result)

    def get_token(self):
        while self.text[self.position]:
            if self.text[self.position].isdigit():
                return Token(INTEGER, self.get_number())
            if self.text[self.position] == PLUS:
                self.position += 1
                self.skip_space()
                return Token(PLUS, '+')
            if self.text[self.position] == MINUS:
                self.position += 1
                self.skip_space()
                return Token(MINUS, '-')
            if self.text[self.position] == MULTIPLICATION:
                self.position += 1
                self.skip_space()
                return Token(MULTIPLICATION, '*')
            if self.text[self.position] == DIVISION:
                self.position += 1
                self.skip_space()
                return Token(DIVISION, '/')
            return Token( EOF, None)


class Interpreter(object):
    def __init__(self, text):
        self.lexer = Lexer(text)
        #self.current_token = self.lexer.get_token()
        #self.right = None

    def error(msg):
        raise Exception(msg)

    
    
        
            
            


if __name__ == "__main__":
    signal.signal(signal.SIGINT, handler)
    while True:
        try:
            text = input("py_calc $> ")
        except EOFError:
            break
        result = Interpreter(text)
        print(result.factor().value)
