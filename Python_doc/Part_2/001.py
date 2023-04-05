INTEGER, PLUS, MINUS, EOF = 'INTEGER', 'PLUS', 'MINUS', 'EOF'
# Token


class Token(object):
    def __init__(self, type, value):
        # token type: INT, PLUS, EOF
        self.type = type
        self.value = value

    def __str__(self):  # string representation
        return 'Token({type}, {value})'.format(type=self.type, value=repr(self.value))

    def __repr__(self):
        return self.__str__()

# Interpreter


class Interpreter(object):
    def __init__(self, text):
        # client string
        self.text = text
        # index of text
        self.position = 0
        # current token
        self.current_token = None
        self.current_char = self.text[self.position]

    def error(self):
        raise Exception('Error parsing input')

    def advance(self):
        # advance the pos pointer and set the current_char variable
        self.position += 1
        if self.position > len(self.text) - 1:
            self.current_char = None  # Indicates end of input
        else:
            self.current_char = self.text[self.position]

    def skip_whitespace(self):
        while self.current_char.isspace():
            self.advance()

    def integer(self):
        result = ''
        while self.current_char is not None and  self.current_char.isdigit():
            result += self.current_char
            self.advance()
        return int(result)

    def get_next_token(self):
        while self.current_char is not None:
            if self.current_char.isspace():
                self.skip_whitespace()
                continue
            if self.current_char.isdigit():
                return Token(INTEGER, self.integer())
            if self.current_char == '+':
                self.advance()
                return Token(PLUS, '+')
            if self.current_char == '-':
                self.advance()
                return Token(MINUS, '-')
            self.error()

    def eat(self, token_type):
        # compare the current token type with past token
        # type and if they match the "eat" the current token
        # and assign the next token to the self.current_token
        # otherwise raise execption
        if self.current_token.type == token_type:
            self.current_token = self.get_next_token()
        else:
            self.error()

    def expr(self):
        # set current token to the first token from input
        self.current_token = self.get_next_token()

        # we expect the current token to be a single-digit integer
        left = self.current_token
        self.eat(INTEGER)

        # we expect the current token to be a '+' or '-'
        op = self.current_token
        if op.type == PLUS:
            self.eat(PLUS)
        else:
            self.eat(MINUS)

        # we expect the current token to be single-digit integer
        right = self.current_token
        self.eat(INTEGER)
        # after the above call the self.current_token is set to EOF

        # at this point INTEGER PLUS INTEGER sequence of tokens
        # has been succefully found and the method can just
        # return the result of adding two integers, this
        # effectively interpreting client input
        if op.type == PLUS:
            result = left.value + right.value
        else:
            result = left.value - right.value
        return result


def main():
    while True:
        try:
            text = input("calcul> ")
        except EOFError:
            break  # break in EOF
        if not text:
            continue
        intepreter = Interpreter(text)
        result = intepreter.expr()
        print(result)


if __name__ == '__main__':
    main()
