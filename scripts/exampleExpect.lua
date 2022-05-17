echo(true)

if spawn(arg[0]..[[\..\..\tppsss.exe]]) then
    timeout = 2

    RC = expect("[y/n]")

    echo(false)

    if RC==0 then
        send("n\r")
    end

    expect("Online")
    send("setup\r")

    expect("[y/n]")
    send("y\r")

    expect("cookie:")
    send(arg[1].."\r")

    expect("steamLoginSecure set")
    send("break\r")
end
