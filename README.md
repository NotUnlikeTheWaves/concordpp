# C++ on Discord/concordpp
**C++** **on** dis**cord** (concordpp) is a work-in-progress Discord library. It's currently in alpha!


The Discord server/guild for this library can be found by clicking the Discord icon below.

[![Foo](https://upload.wikimedia.org/wikipedia/en/thumb/b/b7/Discord_logo_svg.svg/320px-Discord_logo_svg.svg.png)](https://discord.gg/rY3EFpa)

An example project can be found **[here](https://github.com/hiddos/concordpp-example)**, with instructions on how to build.

Please see the issues for what work still needs to be done. If you want to build a stable bot using more than the absolute minimum of Discord's API, this might not be the library for you (yet). **This library is subject to change.**


### Currently implemented
 * WebSockets (all options except voice)
 * Reconnecting/resuming upon disconnects
 * Simple command handler
 * Limited REST endpoints - being worked on, if you need something give me a call.
 * Basic rate limiting handling - won't avoid being limited, but will re-try until possible again.


### To-do
Apart from other things, see the issues list.
