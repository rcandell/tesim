function [array] = reader()

mv1 = load("TE_data_mv1.dat");
mv1 = mv1(1, :);
delete("TE_data_mv1.dat");

mv2 = load("TE_data_mv2.dat");
mv2 = mv2(end, :);
delete("TE_data_mv2.dat");

mv3 = load("TE_data_mv3.dat");
mv3 = mv3(end, :);
delete("TE_data_mv3.dat");

me02 = load("TE_data_me02.dat");
me02 = me02(end, :);
delete("TE_data_me02.dat");

me03 = load("TE_data_me03.dat");
me03 = me03(end, :);
delete("TE_data_me03.dat");

me04 = load("TE_data_me04.dat");
me04 = me04(end, :);
delete("TE_data_me04.dat");

me05 = load("TE_data_me05.dat");
me05 = me05(end, :);
delete("TE_data_me05.dat");

me06 = load("TE_data_me06.dat");
me06 = me06(end, :);
delete("TE_data_me06.dat");

me07 = load("TE_data_me07.dat");
me07 = me07(end, :);
delete("TE_data_me07.dat");

me08 = load("TE_data_me08.dat");
me08 = me08(end, :);
delete("TE_data_me08.dat");

me09 = load("TE_data_me09.dat");
me09 = me09(end, :);
delete("TE_data_me09.dat");

me10 = load("TE_data_me10.dat");
me10 = me10(end, :);
delete("TE_data_me10.dat");

me11 = load("TE_data_me11.dat");
me11 = me11(end, :);
delete("TE_data_me11.dat");

%now combine into one where only the final value is used
array(1:1:4, 1) = mv1';
array(5:1:8, 1) = mv2';
array(9:1:12,1) = mv3';

%combine measured variables except for 1-4 (otherwise duplicated) (matches format of connection matrix)

array(13:1:16, 1) = me02';
array(17:1:20, 1) = me03';
array(20:1:23, 1) = me04';
array(24:1:27, 1) = me05';
array(28:1:31, 1) = me06';
array(32:1:35, 1) = me07';
array(36:1:39, 1) = me08';
array(40:1:43, 1) = me09';
array(44:1:47, 1) = me10';
array(48:1:49, 1) = me11';

end
