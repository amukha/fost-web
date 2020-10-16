/**
    Copyright 2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#pragma once


#include <fost/core>
#include <functional>


namespace fostlib::urlhandler {


    using test_throw_plugin_fn = std::function<void(fostlib::string)>;


    struct test_throw_plugin {
        test_throw_plugin(fostlib::string name, test_throw_plugin_fn);
        ~test_throw_plugin();

      private:
        string name;
    };


}
