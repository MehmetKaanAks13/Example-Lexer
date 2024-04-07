#include <iostream>
#include <string_view>
#include <unordered_map>
#include <cctype>
#include <source_location>
#include <stdexcept>

enum class TokenType {
    // ===== DEFAULT TOKENS ===== //
    Identifier,
    Number,
    Comment,

    // ===== OPERATORS ===== //
    Plus,           // +
    Minus,          // -
    Multiply,       // *
    Divide,         // /
    Modulus,        // %
    Assign,         // =
    LessThan,       // <
    GreaterThan,    // >
    Semicolon,      // ;
    Colon,          // :
    Dot,            // .
    Comma,          // ,

    // ===== BINARY OPERATORS ===== //
    SingleQuote,    // '
    DoubleQuote,    // "
    LeftParenthesis,// (
    LeftBrace,      // {
    LeftBracket,    // [
    RightParenthesis,// )
    RightBrace,     // }
    RightBracket,   // ]

    // ===== Other OPERATORS ===== //
    Slash,          // /
    Backslash,      // '\'
    QuestionMark,   // ?
    ExclamationMark,// !
    DollarSign,     // $
    Ampersand,      // &
    VerticalBar,    // |
    AtSign,         // @
    Caret,          // ^

    // ===== STRUCTURAL ===== //
    Keyword,
    Boolean,
    DataType,

    // ===== Other ===== //
    End,
    Unexpected
};

struct Token {
    TokenType type;
    std::string_view lexeme;
    std::source_location location;
};

class Lexer {
public:
    explicit Lexer(std::string_view input) : input_(input), position_(0) {}

    Token getNextToken() {
        skipWhitespace();

        if (position_ >= input_.size()) {
            return { TokenType::End, "", std::source_location::current() };
        }

        char currentChar = input_[position_];

        if (std::isdigit(currentChar)) {
            return lexNumber();
        } else if (std::isalpha(currentChar) || currentChar == '_') {
            return lexIdentifierOrKeyword();
        } else if (currentChar == '#') {
            return lexComment();
        } else {
            return createOperatorToken(currentChar);
        }
    }

private:
    std::string_view input_;
    size_t position_;

    void skipWhitespace() {
        while (position_ < input_.size() && std::isspace(input_[position_])) {
            ++position_;
        }
    }

    std::unordered_map<std::string_view, TokenType> keywords = {
            {"if", TokenType::Keyword},
            {"else", TokenType::Keyword},
            {"switch", TokenType::Keyword},
            {"case", TokenType::Keyword},
            {"while", TokenType::Keyword},
            {"for", TokenType::Keyword},
            {"break", TokenType::Keyword},
            {"continue", TokenType::Keyword},
            {"return", TokenType::Keyword},
            {"include", TokenType::Keyword}

            // === YOU CAN ADD MORE! === //

    };

    std::unordered_map<std::string_view, TokenType> booleans = {
            {"true", TokenType::Boolean},
            {"false", TokenType::Boolean}

            // === YOU CAN ADD MORE! === //

    };

    std::unordered_map<std::string_view, TokenType> data_types = {
            {"int", TokenType::DataType},
            {"string", TokenType::DataType},
            {"char", TokenType::DataType},
            {"boolean", TokenType::DataType}

            // === YOU CAN ADD MORE! === //

    };

    Token lexNumber() {
        size_t start = position_;
        while (position_ < input_.size() && (std::isdigit(input_[position_]) || input_[position_] == '.')) {
            ++position_;
        }
        return { TokenType::Number, input_.substr(start, position_ - start), std::source_location::current() };
    }

    Token lexIdentifierOrKeyword() {
        size_t start = position_;
        while (position_ < input_.size() && (std::isalnum(input_[position_]) || input_[position_] == '_')) {
            ++position_;
        }
        std::string_view lexeme = input_.substr(start, position_ - start);
        if (keywords.find(lexeme) != keywords.end()) {
            return { keywords[lexeme], lexeme, std::source_location::current() };
        }
        if (booleans.find(lexeme) != booleans.end()) {
            return { booleans[lexeme], lexeme, std::source_location::current() };
        }
        if (data_types.find(lexeme) != data_types.end()) {
            return { data_types[lexeme], lexeme, std::source_location::current() };
        }
        return { TokenType::Identifier, lexeme, std::source_location::current() };
    }


    Token lexComment() {
        size_t start = position_;
        while (position_ < input_.size() && input_[position_] != '\n') {
            ++position_;
        }
        return { TokenType::Comment, input_.substr(start, position_ - start), std::source_location::current() };
    }

    Token createOperatorToken(char currentChar) {
        ++position_;
        switch (currentChar) {
            case '+':
                return { TokenType::Plus, "+", std::source_location::current() };
            case '-':
                return { TokenType::Minus, "-", std::source_location::current() };
            case '*':
                return { TokenType::Multiply, "*", std::source_location::current() };
            case '/':
                return { TokenType::Divide, "/", std::source_location::current() };
            case '%':
                return { TokenType::Modulus, "%", std::source_location::current() };
            case '=':
                return { TokenType::Assign, "=", std::source_location::current() };
            case '(':
                return { TokenType::LeftParenthesis, "(", std::source_location::current() };
            case '{':
                return { TokenType::LeftBrace, "{", std::source_location::current() };
            case '[':
                return { TokenType::LeftBracket, "[", std::source_location::current() };
            case ')':
                return { TokenType::RightParenthesis, ")", std::source_location::current() };
            case '}':
                return { TokenType::RightBrace, "}", std::source_location::current() };
            case ']':
                return { TokenType::RightBracket, "]", std::source_location::current() };
            case ';':
                return { TokenType::Semicolon, ";", std::source_location::current() };
            case ':':
                return { TokenType::Colon, ":", std::source_location::current() };
            case '.':
                return { TokenType::Dot, ".", std::source_location::current() };
            case ',':
                return { TokenType::Comma, ",", std::source_location::current() };
            case '\\':
                return { TokenType::Backslash, "\\", std::source_location::current() };
            case '\'':
                return { TokenType::SingleQuote, "'", std::source_location::current() };
            case '"':
                return { TokenType::DoubleQuote, "\"", std::source_location::current() };
            case '?':
                return { TokenType::QuestionMark, "?", std::source_location::current() };
            case '!':
                return { TokenType::ExclamationMark, "!", std::source_location::current() };
            case '$':
                return { TokenType::DollarSign, "$", std::source_location::current() };
            case '&':
                return { TokenType::Ampersand, "&", std::source_location::current() };
            case '|':
                return { TokenType::VerticalBar, "|", std::source_location::current() };
            case '@':
                return { TokenType::AtSign, "@", std::source_location::current() };
            case '^':
                return { TokenType::Caret, "^", std::source_location::current() };
            case '<':
                return { TokenType::LessThan, "<", std::source_location::current() };
            case '>':
                return { TokenType::GreaterThan, ">", std::source_location::current() };
            default:
                throw std::runtime_error("Unexpected character: " + std::string(1, currentChar));
        }
    }
};

int main() {
    std::string_view input = R"(
    include <iostream>
    # The main function.
    int main() {
        std::cout << "Hello, World!" << std::endl;
        string PI = "3.1415";
        boolean a = true;
        int x = 938649;
        char chr = '32'; # Add code here.
        return 0;
    }
    )";

    Lexer lexer(input);
    Token token;
    do {
        token = lexer.getNextToken();
        switch (token.type) {
            case TokenType::Identifier:
                std::cout << "Identifier";
                break;
            case TokenType::Keyword:
                std::cout << "Keyword";
                break;
            case TokenType::Boolean:
                std::cout << "Boolean";
                break;
            case TokenType::Number:
                std::cout << "Number";
                break;
            case TokenType::DataType:
                std::cout << "DataType";
                break;
            case TokenType::Comment:
                std::cout << "Comment";
                break;
            case TokenType::Plus:
                std::cout << "Plus";
                break;
            case TokenType::Minus:
                std::cout << "Minus";
                break;
            case TokenType::Multiply:
                std::cout << "Multiply";
                break;
            case TokenType::Divide:
                std::cout << "Divide";
                break;
            case TokenType::Modulus:
                std::cout << "Modulus";
                break;
            case TokenType::Assign:
                std::cout << "Assign";
                break;
            case TokenType::LessThan:
                std::cout << "LessThan";
                break;
            case TokenType::GreaterThan:
                std::cout << "GreaterThan";
                break;
            case TokenType::LeftParenthesis:
                std::cout << "LeftParenthesis";
                break;
            case TokenType::LeftBrace:
                std::cout << "LeftBrace";
                break;
            case TokenType::LeftBracket:
                std::cout << "LeftBracket";
                break;
            case TokenType::RightParenthesis:
                std::cout << "RightParenthesis";
                break;
            case TokenType::RightBrace:
                std::cout << "RightBrace";
                break;
            case TokenType::RightBracket:
                std::cout << "RightBracket";
                break;
            case TokenType::Semicolon:
                std::cout << "Semicolon";
                break;
            case TokenType::Colon:
                std::cout << "Colon";
                break;
            case TokenType::Dot:
                std::cout << "Dot";
                break;
            case TokenType::Comma:
                std::cout << "Comma";
                break;
            case TokenType::Slash:
                std::cout << "Slash";
                break;
            case TokenType::Backslash:
                std::cout << "Backslash";
                break;
            case TokenType::SingleQuote:
                std::cout << "SingleQuote";
                break;
            case TokenType::DoubleQuote:
                std::cout << "DoubleQuote";
                break;
            case TokenType::QuestionMark:
                std::cout << "QuestionMark";
                break;
            case TokenType::ExclamationMark:
                std::cout << "ExclamationMark";
                break;
            case TokenType::DollarSign:
                std::cout << "DollarSign";
                break;
            case TokenType::Ampersand:
                std::cout << "Ampersand";
                break;
            case TokenType::VerticalBar:
                std::cout << "VerticalBar";
                break;
            case TokenType::AtSign:
                std::cout << "AtSign";
                break;
            case TokenType::Caret:
                std::cout << "Caret";
                break;
            case TokenType::End:
                std::cout << "\nEnd of the code!";
                break;
            case TokenType::Unexpected:
                std::cout << "Unexpected";
                break;
        }
        if (token.type != TokenType::End) {
            std::cout << " => |" << token.lexeme << "| \t\tLine: " << token.location.line() << ", Column: " << token.location.column() << std::endl;
        }
    } while (token.type != TokenType::End);

    return 0;
}
