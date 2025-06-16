echo Running Polygon tests...
echo. > results.txt

echo TEST1: U_Shape_Inside >> results.txt
if exist output.txt del output.txt
Polygon.exe test01.txt >> results.txt 2>&1
if exist output.txt type output.txt >> results.txt
echo. >> results.txt

echo TEST2: U_Shape_Cavity >> results.txt
if exist output.txt del output.txt
Polygon.exe test02.txt >> results.txt 2>&1
if exist output.txt type output.txt >> results.txt
echo. >> results.txt

echo TEST3: C_Shape_Inside >> results.txt
if exist output.txt del output.txt
Polygon.exe test03.txt >> results.txt 2>&1
if exist output.txt type output.txt >> results.txt
echo. >> results.txt

echo TEST4: C_Shape_Hole>> results.txt
if exist output.txt del output.txt
Polygon.exe test04.txt >> results.txt 2>&1
if exist output.txt type output.txt >> results.txt
echo. >> results.txt

echo TEST5: L_Shape_Inside >> results.txt
if exist output.txt del output.txt
Polygon.exe test05.txt >> results.txt 2>&1
if exist output.txt type output.txt >> results.txt
echo. >> results.txt

echo TEST6: Star_Indent_Outside >> results.txt
if exist output.txt del output.txt
Polygon.exe test06.txt >> results.txt 2>&1
if exist output.txt type output.txt >> results.txt
echo. >> results.txt

echo TEST7: neg_frac_U_inside >> results.txt
if exist output.txt del output.txt
Polygon.exe test07.txt >> results.txt 2>&1
if exist output.txt type output.txt >> results.txt
echo. >> results.txt

echo TEST8: rectangle_neg_outside >> results.txt
if exist output.txt del output.txt
Polygon.exe test08.txt >> results.txt 2>&1
if exist output.txt type output.txt >> results.txt
echo. >> results.txt

echo TEST09: large20_inside >> results.txt
if exist output.txt del output.txt
Polygon.exe test09.txt >> results.txt 2>&1
if exist output.txt type output.txt >> results.txt
echo. >> results.txt

echo TEST10: large20_outside >> results.txt
if exist output.txt del output.txt
Polygon.exe test10.txt >> results.txt 2>&1
if exist output.txt type output.txt >> results.txt
echo. >> results.txt

echo All tests completed. Results saved to results.txt

