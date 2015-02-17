while 1
    try
        sim('test_udp.slx')
    catch e
        disp(e)
        pause(5)
    end
end