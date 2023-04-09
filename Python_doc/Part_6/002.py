import signal

INTEGER, PLUS, MINUS, MUL, DIV, LPARENT, RPARENT, EOF = (
    'INTEGER', 'PLUS', 'MINUS', 'MUL', 'DIV', '(', ')', 'EOF')


def handler(sig, frame):
    exit(0)


class Token(object):
    def __init__(self, type, value):
        self.type = type
        self.value = value

    def __str__(self):
        return 'Token({type}, {value})'.format(type=self.type, vale=self.value)

    def __repr__(self):
        return self.__str__()


class Lexer(object):
    def __init__(self, text):
        self.text = text
        self.position = 0

    def error(self):
        raise Exception("Invalid character")

    def advance(self):
        if self.position < len(self.text) - 1:
            self.position += 1

    def skip_white_space(self):
        while self.text[self.position] is not None and self.text[self.position].isspace():
            self.advance()

    def integer(self):
        result = ''
        while self.text[self.position] is not None and self.text[self.position].isdigit():
            result += self.text[self.position]
            self.advance()
        return (int(result))

    def get_next_token(self):
        while self.text[self.position]:
            if self.text[self.position].isspace():
                self.skip_white_space()
                continue
            if self.text[self.position].isdigit():
                return Token(INTEGER, self.integer())
            if self.text[self.position] == '*':
                self.advance()
                return Token(MUL, '*')
            if self.text[self.position] == '/':
                self.advance()
                return Token(DIV, '/')
            if self.text[self.position] == '+':
                self.advance()
                return Token(PLUS, '+')
            if self.text[self.position] == '-':
                self.advance()
                return Token(MINUS, '-')
            self.error()
        return Token(EOF, None)


class Interpreter(object):
    def __init__(self, lexer):
        self.lexer = lexer
        self.current_token = self.lexer.get_next_token()

    def error(self):
        raise Exception("Invalid syntax")

    # move to next token if found this token
    # otherwise throw error
    def skip_token(self, token_type):
        if self.current_token.type == token_type:
            self.current_token = self.lexer.get_next_token()
        else:
            self.error()

    # factor: INTEGER | expr
    def factor(self):
        token = self.current_token
        print("token is " , token.value, "it's type is ", token.type)
        if token.type == INTEGER:
            self.skip_token(INTEGER)
            print("after skiping token")
            print("self.token is ", self.current_token.value, " it's type is ",self.current_token.type)
            return token.value

    # term: factor ((MUL | DIV) factor)
    def term(self):
        print("get factor")
        result = self.factor()
        while self.current_token.type in (MUL, DIV):
            token = self.current_token
            if token.type == MUL:
                self.skip_token(MUL)
                result *= self.factor()
            elif token.type == DIV:
                self.skip_token(DIV)
                result /= self.factor()
        print("term will return value: ", result)
        return result

    # expr: term ((PLUS | MINUS) term)*
    def expr(self):
        print("get term")
        result = self.term()
        while self.current_token.type in (PLUS, MINUS):
            token = self.current_token
            if token.type == PLUS:
                self.skip_token(PLUS)
                result += self.term()
            elif token.type == MINUS:
                self.skip_token(MINUS)
                result -= self.term()
        return result


def main():
    signal.signal(signal.SIGINT, handler)
    while True:
        try:
            text = input("pycalc $> ")
        except EOFError:
            break
        if text:
            print("receive: ", text)
            lexer = Lexer(text)
            interpreter = Interpreter(lexer)
            result = interpreter.expr()
            print(result)


if __name__ == "__main__":
    main()
