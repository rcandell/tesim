time = load("timedata.dat");
rchange = load("statechangedata.dat");

size(rchange)
arr(:,1) = rchange(1, :)';
arr(:,2) = rchange(end, :)';
arr

figure(1)
plot(rchange(:,1:12))
figure(2)
plot(rchange(:,16:20))
pause(10)

delete statechangedata.dat
delete timedata.dat

