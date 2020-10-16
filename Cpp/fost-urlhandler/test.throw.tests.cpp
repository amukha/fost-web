/**
    Copyright 2019-2020 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include <fost/urlhandler>
#include <fost/test>

#include <stdexcept>


FSL_TEST_SUITE(throw_exception);


FSL_TEST_FUNCTION(empty_config) {
    fostlib::json config;
    fostlib::insert(config, "view", "test.throw");
    fostlib::http::server::request req;
    try {
        auto const response{fostlib::urlhandler::view::execute(
                config, "", req, fostlib::host{})};
        FSL_CHECK(false);
    } catch (const std::invalid_argument &) {}
}


FSL_TEST_FUNCTION(logic_error) {
    fostlib::json config;
    fostlib::insert(config, "view", "test.throw");
    fostlib::insert(config, "configuration", "exception", "std::logic_error");
    fostlib::http::server::request req;
    try {
        auto const response{fostlib::urlhandler::view::execute(
                config, "", req, fostlib::host{})};
        FSL_CHECK(false);
    } catch (const std::logic_error &e) {
        FSL_CHECK_EQ(
                e.what(),
                std::string{"Test exception message from test.throw"});
    }
    insert(config, "configuration", "message", "Message set");
    try {
        auto const response{fostlib::urlhandler::view::execute(
                config, "", req, fostlib::host{})};
        FSL_CHECK(false);
    } catch (const std::logic_error &e) {
        FSL_CHECK_EQ(e.what(), std::string{"Message set"});
    }
}
