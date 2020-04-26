for FILE in `find . -type f -name "*.dot"`
do
	dot -Tpng $FILE -o $FILE.png
done