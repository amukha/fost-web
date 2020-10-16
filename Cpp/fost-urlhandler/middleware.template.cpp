/**
    Copyright 2011-2020 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include "fost-urlhandler.hpp"
#include <fost/urlhandler.hpp>


namespace {


    fostlib::nullable<fostlib::string> find_content(
            const fostlib::string &text, const fostlib::string &tag) {
        fostlib::string::size_type start(text.find("<" + tag));
        if (start != fostlib::string::npos) {
            start = text.find(">", start) + 1;
            fostlib::string::size_type end(text.find("</" + tag));
            if (end != fostlib::string::npos)
                return text.substr(start, end - start);
        }
        return fostlib::null;
    }


    void replace_content(
            fostlib::string &skin,
            const fostlib::string &block,
            const fostlib::string &with) {
        fostlib::string::size_type start = skin.find(block);
        if (start != fostlib::string::npos) {
            skin.replace(start, block.code_points(), with);
        }
    }


    const class middleware_template : public fostlib::urlhandler::view {
      public:
        middleware_template() : view("fost.middleware.template") {}

        std::pair<boost::shared_ptr<fostlib::mime>, int> operator()(
                const fostlib::json &configuration,
                const fostlib::string &path,
                fostlib::http::server::request &req,
                const fostlib::host &h) const {
            fostlib::string view(
                    fostlib::coerce<fostlib::string>(configuration["view"]));
            std::pair<fostlib::string, fostlib::json> view_fn =
                    view::find_view(view);
            std::pair<boost::shared_ptr<fostlib::mime>, int> wrapped(
                    view_for(view_fn.first)(view_fn.second, path, req, h));
            if (wrapped.first->headers()["Content-Type"].value()
                == "text/html") {
                fostlib::string skin(fostlib::utf::load_file(
                        fostlib::coerce<fostlib::fs::wpath>(
                                configuration["template"])));
                fostlib::string content(
                        fostlib::coerce<fostlib::string>(*wrapped.first));

                fostlib::nullable<fostlib::string> title(
                        find_content(content, "title"));
                if (title)
                    replace_content(
                            skin, "{{ element title }}",
                            "<title>" + title.value() + "</title>");

                fostlib::nullable<fostlib::string> inner(
                        find_content(content, "body"));
                if (inner)
                    replace_content(skin, "{{ content body }}", inner.value());

                boost::shared_ptr<fostlib::mime> response(
                        new fostlib::text_body(
                                skin, wrapped.first->headers(), "text/html"));
                return std::make_pair(response, wrapped.second);
            } else
                return wrapped;
        }
    } c_middleware_template;


}
