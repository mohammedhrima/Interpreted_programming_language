import signal


def handler(sig, frame):
    exit(0)


INTEGER, PLUS, MINUS, MUL, DIV, LPARENT, RPARENT, EOF = (
    'INTEGER', 'PLUS', 'MINUS', 'MUL', 'DIV', '(', ')', 'EOF')


class Token(object):
    def __init__(self, type, value):
        self.type = type
        self.value = value

    def __str__(self):
        return 'Token({type}, {value})'.format(type=self.type, value=self.value)

    def __repr__(self):
        return self.__str__()


class Lexer(object):
    def __init__(self, text):
        self.text = text
        self.position = 0
        self.current_char = self.text[self.position]

    def error(self):
        raise Exception("Invalid character")

    def advance(self):
        self.position += 1
        if self.position > len(self.text) - 1:
            self.current_char = None
        else:
            self.current_char = self.text[self.position]

    def skip_white_space(self):
        while self.current_char is not None and self.current_char.isspace():
            self.advance()

    def integer(self):
        result = ''
        while self.current_char is not None and self.current_char.isdigit():
            result += self.current_char
            self.advance()
        return (int(result))

    def get_next_token(self):
        while self.current_char is not None:
            if self.current_char.isspace():
                self.skip_white_space()
                continue
            if self.current_char.isdigit():
                return Token(INTEGER, self.integer())
            if self.current_char == '*':
                self.advance()
                return Token(MUL, '*')
            if self.current_char == '/':
                self.advance()
                return Token(DIV, '/')
            if self.current_char == '+':
                self.advance()
                return Token(PLUS, '+')
            if self.current_char == '-':
                self.advance()
                return Token(MINUS, '-')
            if self.current_char == '(':
                self.advance()
                return Token(LPARENT, '(')
            if self.current_char == ')':
                self.advance()
                return Token(RPARENT, ')')
            self.error()
        return Token(EOF, None)


class Interpreter(object):
    def __init__(self, lexer):
        self.lexer = lexer
        self.current_token = self.lexer.get_next_token()

    def error(self):
        raise Exception("Invalid syntax")

    def eat(self, token_type):
        if self.current_token.type == token_type:
            self.current_token = self.lexer.get_next_token()
        else:
            self.error()

    # factor : INTEGER
    def factor(self):
        token = self.current_token
        if token.type == INTEGER:
            self.eat(INTEGER)
            return token.value
        elif token.type == LPARENT:
            self.eat(LPARENT)
            result = self.expr()
            self.eat(RPARENT)
            return result

    # term : factor ((MUL | DIV) factor)
    def term(self):
        result = self.factor()
        while self.current_token.type in (MUL, DIV):
            token = self.current_token
            if token.type == MUL:
                self.eat(MUL)  # skip * and spaces
                result = result * self.factor()  # get the number there
            elif token.type == DIV:
                self.eat(DIV)  # skip / and spaces
                result = result / self.factor()  # get the number there
        return result

    # expr : term ((PLUS | MINUS) term)*
    def expr(self):
        result = self.term()
        while self.current_token.type in (PLUS, MINUS):
            token = self.current_token
            if token.type == PLUS:
                self.eat(PLUS)  # skip * and spaces
                result = result + self.term()  # get the number there
            elif token.type == MINUS:
                self.eat(MINUS)  # skip / and spaces
                result = result - self.term()  # get the number there
        return result


def main():
    signal.signal(signal.SIGINT, handler)
    while True:
        try:
            text = input("py_calc $> ")
        except EOFError:
            break
        if not text:
            continue
        lexer = Lexer(text)
        interpreter = Interpreter(lexer)
        result = interpreter.expr()
        print(result)


if __name__ == "__main__":
    main()
