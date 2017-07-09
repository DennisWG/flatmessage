#include <flatmessage/ast/printer.hpp>
#include <flatmessage/compiler.hpp>
#include <flatmessage/generator/template_generator.hpp>
#include <flatmessage/parser.hpp>
#include <flatmessage/parser/config.hpp>
#include <flatmessage/parser/error_handler.hpp>
#include <flatmessage/parser/expression.hpp>

#include <boost/filesystem.hpp>

namespace flatmessage::parser
{
    std::optional<ast::ast> parse_string(std::string const & content, std::string& out_error, std::string const& source)
    {
        using flatmessage::parser::error_handler_type;
        using flatmessage::parser::iterator_type;
        using boost::spirit::x3::with;
        using boost::spirit::x3::ascii::space;

        iterator_type iter(content.begin());
        iterator_type end(content.end());

        std::stringstream out;
        error_handler_type error_handler(iter, end, out, source);

        auto const parser = with<flatmessage::parser::error_handler_tag>(std::ref(error_handler))[+(
            flatmessage::message() | flatmessage::enumeration() | flatmessage::data() | flatmessage::module_decl()
            | flatmessage::import_decl() | flatmessage::protocol_decl())];

        using result_type = flatmessage::ast::ast;

        result_type result;
        bool success = flatmessage::x3::phrase_parse(iter, end, parser, space, result);

        if (success)
        {
            if (iter != end)
            {
                out_error = "Error! Expecting end of input here: " + std::string(iter, end) + '\n';
                return {};
            }
        }
        else
        {
            out_error = out.str();
            return {};
        }

        return result;
    }

    std::optional<ast::ast> parse_file(boost::filesystem::path const& file_path, std::string& out_error)
    {
        std::ifstream file(file_path.string());
        if (!file)
        {
            out_error = "Invalid file path \"" + file_path.string() + "\"";
            return {};
        }

        auto content = std::string{(std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()};

        return parse_string(content, out_error, file_path.string());
    }
}