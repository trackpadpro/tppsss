#!/usr/bin/expect

set SCRIPT_DIR [file dirname $argv0]
set TPPSSS_DIR [file dirname $SCRIPT_DIR]
set STEAM_LOGIN_COOKIE [lindex $argv 0]

spawn $TPPSSS_DIR/tppsss

set timeout 2

expect "y/n" {
    send "n\r"
}

#Online
expect "$$$>" {
    send "fetch\r"
}

expect "$$$>" {
    send "setup\r"
}

expect "sss>" {
    send "steam\r"
}

expect "y/n" {
    send "y\r"
}

expect "content of the cookie" {
    send "$STEAM_LOGIN_COOKIE\r"
}

expect "sss>" {
    send "asset\r"
}

expect "y/n" {
    send "y\r"
}

expect "skip" {
    send "skip\r"
}

expect "sss>" {
    send "leave\r"
}

expect "$$$>" {
    send "fetch\r"
}

expect "$$$>" {
    send "break\r"
}

interact
