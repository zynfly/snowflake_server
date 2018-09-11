//
// Created by zynfly on 2018/9/6.
//

#include "Handle.h"

Handle::Handle(nlohmann::json config)
{
    _snow_flake_map.emplace(
            std::make_pair(
                    std::string("__default__"),
                    std::shared_ptr<SnowFlakeInterface>(
                            new MilSnowFlake(
//                                    static_cast<uint64_t>(
//                                            std::chrono::duration_cast<std::chrono::milliseconds>(
//                                                    std::chrono::steady_clock::now().time_since_epoch()
//                                            ).count()
//                                    ),
                                    0,
                                    0,
                                    0,
                                    41,
                                    5,
                                    5,
                                    12
                            )
                    )
            )
    );
}

void Handle::handleRequest(Request const &req, Reply &rep)
{
    static nlohmann::json json;

    static auto snow_flake_ptr = _snow_flake_map.find("__default__")->second;
    auto        search_iter    = _snow_flake_map.find(req.uri);
    if (_snow_flake_map.end() != search_iter) {
        snow_flake_ptr = search_iter->second;
    }


    json["id"] = snow_flake_ptr->nextId();

    rep.status  = Reply::ok;
    rep.content = json.dump();
    rep.headers.emplace_back("Content-Type", "application/json;charset=utf-8");
}
