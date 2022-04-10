echo(true)
if spawn([[..\..\tppsss\tppsss.exe]]) then
    timeout = 2
    RC = expect("[y/n]")
    timeout = 3600
    echo(false)
    if RC==0 then
        send("n\r")
    end
    expect("Online")
    send("setup\r")
    expect("cookie:")
    send(arg[1].."\r")
    expect("steamLoginSecure set")
    send("break\r")
end
