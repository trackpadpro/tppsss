echo(true)

if spawn(arg[0]..[[\..\..\tppsss.exe]]) then
    timeout = 2

    RC = expect("[y/n]")

    echo(false)

    if RC==0 then
        send("n\r")
    end

    expect("$$$>") --Online
    send("fetch\r") --Fetch to fail history without Steam cookie if not yet set

    expect("$$$>")
    send("setup\r")

    expect("sss>") --Settings
    send("steam\r")

    expect("[y/n]")
    send("y\r")

    expect("cookie:")
    send(arg[1].."\r")

    expect("sss>") --Even if the cookie is invalid, user should return to settings
    send("asset\r")

    expect("[y/n]")
    send("y\r")

    expect("skip]?")
    send("skip\r")

    expect("sss>")
    send("leave\r")

    expect("$$$>")
    send("fetch\r")

    expect("$$$>")
    send("break\r")
end
