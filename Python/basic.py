# CONSTANT
DIGITS = "0123456789"

# Error


class Error:
    def __init__(self, position_start, position_end, error_name, details):
        self.position_start = position_start
        self.position_end = position_end
        self.error_name = error_name
        self.details = details

    def as_string(self):
        result = f"{self.error_name}: {self.details}\nFile {self.position_start.filename}, line {self.position_start.line + 1}"
        return result


class IllegalCharError(Error):
    def __init__(self, position_start, position_end, details):
        super().__init__(position_start, position_end, "Illegal Character", details)


# POSTION


class Position:
    def __init__(self, index, line, column, filename, filetext):
        self.index = index
        self.line = line
        self.column = column
        self.filename = filename
        self.filetext = filetext

    def advance(self, current_char):
        self.index += 1
        self.column += 1
        if current_char == "\n":
            self.line += 1
            self.column = 0
        return self

    def copy(self):
        return Position(
            self.index, self.line, self.column, self.filename, self.filetext
        )


# TOKENS
TT_INT = "INT"
TT_FLOAT = "FLOT"
TT_PLUS = "PLUS"
TT_MINUS = "MINUS"
TT_MUL = "MUL"
TT_DIV = "DIV"
TT_LPAREN = "LPAREN"
TT_RPAREN = "RPAREN"


class Token:
    def __init__(self, type_, value=None):
        self.type = type_
        self.value = value

    def __repr__(self):
        if self.value:
            return f"{self.type}:{self.value}"
        return f"{self.type}"


# LEXER


class Lexer:
    def __init__(self, filename, text):
        self.text = text
        self.position = Position(-1, 0, -1, filename, text)
        self.current_char = None
        self.advance()

    def advance(self):
        self.position.advance(self.current_char)  # move to next character
        self.current_char = (
            self.text[self.position.index]
            if self.position.index < len(self.text)
            else None
        )

    def make_tokens(self):
        tokens = []
        while self.current_char != None:
            if self.current_char in " \t":  # check if is space or tab
                self.advance()
            elif self.current_char in DIGITS:
                tokens.append(self.make_number())
            elif self.current_char == "+":
                tokens.append(Token(TT_PLUS))
                self.advance()
            elif self.current_char == "-":
                tokens.append(Token(TT_MINUS))
                self.advance()
            elif self.current_char == "*":
                tokens.append(Token(TT_MUL))
                self.advance()
            elif self.current_char == "/":
                tokens.append(Token(TT_DIV))
                self.advance()
            elif self.current_char == "(":
                tokens.append(Token(TT_LPAREN))
                self.advance()
            elif self.current_char == ")":
                tokens.append(Token(TT_RPAREN))
                self.advance()
            else:
                # return error
                position_start = self.position.copy()
                char = self.current_char
                self.advance()
                return [], IllegalCharError(
                    position_start, self.position, "'" + char + "'"
                )
        return tokens, None

    def make_number(self):
        num_str = ""
        dot_count = 0
        while self.current_char != None and self.current_char in DIGITS + ".":
            if self.current_char == ".":
                if dot_count == 1:
                    break
                dot_count += 1
                num_str += "."
            else:
                num_str += self.current_char
            self.advance()
        if dot_count == 0:
            return Token(TT_INT, int(num_str))
        else:
            return Token(TT_FLOAT, float(num_str))


# NODES


class NumberNode:
    def __init__(self, token):
        self.token = token

    def __repr__(self):
        return f"{self.token}"


class BinOpNode:
    def __init__(self, left_node, operator_token, right_node):
        self.left_node = left_node
        self.right_node = right_node
        self.operator_token = operator_token

    def __repr__(self):
        return f"({self.left_node}, {self.operator_token}, {self.right_node})"

# PARSER


class Parser:
    def __init__(self, tokens):
        self.tokens = tokens
        self.token_index = 1
        self.advance()

    def advance(self):
        self.token_index += 1
        if self.token_index < len(self.tokens):
            self.current_token = self.tokens[self.token_index]
        return self.current_token
    ###########################################################

    def parse(self):
        res = self.expr()
        return (res)

    def factor(self):
        token = self.current_token
        # print("line 184 " + token)
        if token.type in (TT_INT, TT_FLOAT):
            self.advance()
            return NumberNode(token)

    def term(self):
        left = self.factor()
        # print("line 191 " + left.token)
        while self.current_token.type in (TT_MUL, TT_DIV):
            operation_token = self.current_token
            self.advance()
            right = self.factor()
            left = BinOpNode(left, operation_token, right)
        return left

    def expr(self):
        left = self.term()
        # print("line 201 " + left.token)
        while self.current_token.type in (TT_PLUS, TT_MINUS):
            operation_token = self.current_token
            self.advance()
            right = self.term()
            left = BinOpNode(left, operation_token, right)
        return left


# RUN FUNCTION


def run(filename, text):
    # Genearte Tokens
    lexer = Lexer(filename, text)
    tokens, error = lexer.make_tokens()
    if error:
        return None, error
    # Generate abstract syntax tree
    parser = Parser(tokens)
    ast = parser.parse()

    return ast, None
