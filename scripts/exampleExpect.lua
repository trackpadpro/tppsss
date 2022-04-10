echo(true)
if spawn([[..\..\tppsss\tripleS.exe]]) then
    expect("[y/n]")
    echo(false)
    send("n\r")
    expect("Online")
    send("setup\r")
    expect("cookie:")
    send("STEAM_LOGIN_COOKIE\r")
    expect("steamLoginSecure set")
    send("break\r")
end
