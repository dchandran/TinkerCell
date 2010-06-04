#ifndef TINKERCELL_TC_MAIN_API_H
#define TINKERCELL_TC_MAIN_API_H

#include "../TCstructs.h"

/*! 
 \brief get all visible items
 \ingroup Get items
*/
TCAPIEXPORT ArrayOfItems tc_allItems();

/*! 
 \brief get all selected items
 \ingroup Get items
*/
TCAPIEXPORT ArrayOfItems tc_selectedItems();

/*!
 \brief get all items of the given family items
 \ingroup Get items
*/
TCAPIEXPORT ArrayOfItems tc_itemsOfFamily(String family);

/*! 
 \brief get subset of items that belong to the given family
 \ingroup Get items
*/
TCAPIEXPORT ArrayOfItems tc_itemsOfFamilyFrom(String family, ArrayOfItems itemsToSelectFrom);

/*! 
 \brief get the first item with the given name (full name)
 \ingroup Get items
*/
TCAPIEXPORT Item tc_find(String fullname);

/*! 
 \brief get all items with the given names (full names)
 \ingroup Get items
*/
TCAPIEXPORT ArrayOfItems tc_findItems(ArrayOfStrings names);

/*! 
 \brief select an item
 \ingroup Get items
*/
TCAPIEXPORT void tc_select(Item item);

/*! 
 \brief deselect all items
 \ingroup Get items
*/
TCAPIEXPORT void tc_deselect();

/*! 
 \brief get the name of an item
 \ingroup Get items
*/
TCAPIEXPORT String tc_getName(Item item);

/*! 
 \brief get the full name of an item
 \ingroup Get items
*/
TCAPIEXPORT String tc_getUniqueName(Item item);

/*! 
 \brief set the name of an item (not full name)
 \ingroup Get items
*/
TCAPIEXPORT void tc_rename(Item item,String name);

/*! 
 \brief get the names of several items
 \ingroup Get items
*/
TCAPIEXPORT ArrayOfStrings tc_getNames(ArrayOfItems items);

/*! 
 \brief get the full names of several items
 \ingroup Get items
*/
TCAPIEXPORT ArrayOfStrings tc_getUniqueNames(ArrayOfItems items);

/*! 
 \brief get the family name of an item
 \ingroup Annotation
*/
TCAPIEXPORT String tc_getFamily(Item item);

/*! 
 \brief check is an item belongs in a family (or in a sub-family)
 \ingroup Annotation
*/
TCAPIEXPORT int tc_isA(Item item,String family);

/*! 
 \brief show text in the output window.
 \ingroup Input and Output
*/
TCAPIEXPORT void tc_print(String text);

/*! 
 \brief show error text in the output window.
 \ingroup Input and Output
*/
TCAPIEXPORT void tc_errorReport(String text);

/*! 
 \brief show table in the output window.
 \ingroup Input and Output
*/
TCAPIEXPORT void tc_printTable(TableOfReals data);

/*! 
 \brief show file contents in the output window. 
 \ingroup Input and Output
*/
TCAPIEXPORT void tc_printFile(String filename);

/*! 
 \brief cleat the contents in the output window. 
 \ingroup Input and Output
*/
TCAPIEXPORT void tc_clear();

/*! 
 \brief delete an item
 \ingroup Insert and remove
*/
TCAPIEXPORT void tc_remove(Item item);

/*! 
 \brief get the x location of an item
 \ingroup Appearance
*/
TCAPIEXPORT double tc_getY(Item item);

/*! 
 \brief get the y location of an item
 \ingroup Appearance
*/
TCAPIEXPORT double tc_getX(Item item);

/*! 
 \brief get the y location of a list item. Output is a N x 2 matrix
 \ingroup Appearance
*/
TCAPIEXPORT TableOfReals tc_getPos(ArrayOfItems items);

/*! 
 \brief set the x and y location of an item
 \ingroup Appearance
*/
TCAPIEXPORT void tc_setPos(Item item,double x,double y);

/*! 
 \brief set the x and y location of a list of N items. Input a matrix of positions, with N rows and 2 columns (x,y)
 \ingroup Appearance
*/
TCAPIEXPORT void tc_setPosMulti(ArrayOfItems items, TableOfReals positions);

/*! 
 \brief move all the selected items by a given amount
 \ingroup Appearance
*/
TCAPIEXPORT void tc_moveSelected(double dx,double dy);

/*! 
 \brief is this running in MS windows?
 \ingroup System information
*/
TCAPIEXPORT int tc_isWindows();

/*! 
 \brief is this running in a Mac?
 \ingroup System information
*/
TCAPIEXPORT int tc_isMac();

/*! 
 \brief is this running in Linux?
 \ingroup System information
*/
TCAPIEXPORT int tc_isLinux();

/*! 
 \brief TinkerCell application folder
 \ingroup System information
*/
TCAPIEXPORT String tc_appDir();

/*! 
 \brief create an input window that can call a dynamic library
 \ingroup Input and Output
*/
TCAPIEXPORT void tc_createInputWindowFromFile(TableOfReals input, String filename,String functionname, String title);

/*!
 \brief create an input window that can call a dynamic library
 \ingroup Input and Output
*/
TCAPIEXPORT void tc_createInputWindow(TableOfReals input, String title, void (*f)(TableOfReals));

/*! 
 \brief add options to an existing input window at the i,j-th cell. Options will appear in a list
 \ingroup Input and Output
*/
TCAPIEXPORT void tc_addInputWindowOptions(String title, int i, int j, ArrayOfStrings options);

/*! 
 \brief add a yes or no type of option to an existing input window at the i,j-th cell
 \ingroup Input and Output
*/
TCAPIEXPORT void tc_addInputWindowCheckbox(String title, int i, int j);

/*! 
 \brief open a new graphics window
 \ingroup Input and Output
*/
TCAPIEXPORT void tc_openNewWindow(String title);
/*! 
 \brief get child items of the given item
 \ingroup Get items
*/
TCAPIEXPORT ArrayOfItems tc_getChildren(Item o);

/*! 
 \brief get parent item of the given item
 \ingroup Get items
*/
TCAPIEXPORT Item tc_getParent(Item o);

/*! 
 \brief get the entire data matrix for the given numerical data table of the given item
 \ingroup Network data
*/
TCAPIEXPORT TableOfReals tc_getNumericalData(Item item,String data);

/*! 
 \brief set a new data matrix for an item. Use 0 for the global model item.
 \ingroup Network data
*/
TCAPIEXPORT void tc_setNumericalData(Item o,String title,TableOfReals data);

/*! 
 \brief get the entire data matrix for the given strings data table of the given item
 \ingroup Network data
*/
TCAPIEXPORT TableOfStrings tc_getTextData(Item item,String data);

/*! 
 \brief set the entire data matrix for the given strings data table of the given item
 \ingroup Network data
*/
TCAPIEXPORT void tc_setTextData(Item o,String title,TableOfStrings data);

/*! 
 \brief get all the numeric data table names for the given item. Use 0 for the global tables.
 \ingroup Network data
*/
TCAPIEXPORT ArrayOfStrings tc_getNumericalDataNames(Item o);

/*! 
 \brief get all the text data table names for the given item. Use 0 for the global tables.
 \ingroup Network data
*/
TCAPIEXPORT ArrayOfStrings tc_getTextDataNames(Item o);

/*! 
 \brief zoom by the given factor (0 - 1)
 \ingroup Input and Output
*/
TCAPIEXPORT void tc_zoom(double factor);

/*! 
 \brief get a text from the user (dialog)
 \ingroup Input and Output
*/
TCAPIEXPORT String tc_getString(String title);
/*! 
 \brief get a file from the user (dialog)
 \ingroup Input and Output
*/
TCAPIEXPORT String tc_getFilename();

/*! 
 \brief get a text from the user (dialog) from a list of selections
 \ingroup Input and Output
*/
TCAPIEXPORT int tc_getStringFromList(String title, ArrayOfStrings list,String selectedString);
/*! 
 \brief get a number from the user (dialog)
 \ingroup Input and Output
*/
TCAPIEXPORT double tc_getNumber(String title);

/*! 
 \brief get a list of numbers from the user (dialog) into the argument array
 \ingroup Input and Output
*/
TCAPIEXPORT void tc_getNumbers(ArrayOfStrings labels, double* result);

/*! 
 \brief display a dialog with a text and a yes and no button
 \param String displayed message or question
 \ingroup Input and Output
*/
TCAPIEXPORT int tc_askQuestion(String message);

/*! 
 \brief display a dialog with a text message and a close button
 \param String displayed message
 \ingroup Input and Output
*/
TCAPIEXPORT void tc_messageDialog(String message);

/*!
 \brief get pointer to the current thread
 \ingroup Programming
*/
TCAPIEXPORT Item tc_thisThread();

/*!
 \brief create a window with several sliders. when the sliders change, the given function will be called with the values in the sliders
 \ingroup Input and Output
*/
TCAPIEXPORT void tc_createSliders(TableOfReals input, void (*f)(TableOfReals));

/*! 
 \brief get the color of the item
 \ingroup Appearance
*/
TCAPIEXPORT String tc_getColor(Item item);

/*! 
 \brief set the rgb color  of the item and indicate whether or not the color is permanenet
 \ingroup Appearance
*/
TCAPIEXPORT void tc_setColor(Item item,String name, int permanent);

/*! 
 \brief change the graphics file for drawing one of the nodes
 \ingroup Appearance
*/
TCAPIEXPORT void tc_changeNodeImage(Item item,String filename);

/*! 
 \brief change the graphics file for drawing the arrowheads for the given connection
 \ingroup Appearance
*/
TCAPIEXPORT void tc_changeArrowHead(Item connection,String filename);

/*!
 \brief Change the size of an item
 \ingroup Appearance
*/
TCAPIEXPORT void tc_setSize(Item item,double width,double height,int permanent);

/*!
 \brief get the width of an item
 \ingroup Appearance
*/
TCAPIEXPORT double tc_getWidth(Item item);

/*!
 \brief get the width of an item
 \ingroup Appearance
*/
TCAPIEXPORT double tc_getHeight(Item item);

/*!
 \brief get the width of an item
 \ingroup Appearance
*/
TCAPIEXPORT void tc_setAngle(Item item, double t,int permanent);

/*!
 \brief get the angle of an item
 \ingroup Appearance
*/
TCAPIEXPORT double tc_getAngle(Item item);

/*! 
 \brief initialize main
 \ingroup init
*/
TCAPIEXPORT void tc_Main_api_initialize(
	    ArrayOfItems (*tc_allItems0)(),
		ArrayOfItems (*tc_selectedItems0)(),
		ArrayOfItems (*tc_itemsOfFamily0)(String),
		ArrayOfItems (*tc_itemsOfFamily1)(String, ArrayOfItems),
		Item (*tc_find0)(String),
		ArrayOfItems (*tc_findItems0)(ArrayOfStrings),
		void (*tc_select0)(Item),
		void (*tc_deselect0)(),
		String (*tc_getName0)(Item),
		String (*tc_getUniqueName0)(Item),
		void (*tc_setName0)(Item item,String name),
		ArrayOfStrings (*tc_getNames0)(ArrayOfItems),
		ArrayOfStrings (*tc_getUniqueNames0)(ArrayOfItems),
		String (*tc_getFamily0)(Item),
		int (*tc_isA0)(Item,String),

		void (*tc_clearText)(),
		void (*tc_outputText0)(String),
		void (*tc_errorReport0)(String),
		void (*tc_outputTable0)(TableOfReals),
		void (*tc_printFile0)(String),

		void (*tc_removeItem0)(Item),

		double (*tc_getY0)(Item),
		double (*tc_getX0)(Item),
		TableOfReals (*tc_getPos0)(ArrayOfItems),
		void (*tc_setPos0)(Item,double,double),
		void (*tc_setPos1)(ArrayOfItems,TableOfReals),
		void (*tc_moveSelected0)(double,double),

		int (*tc_isWindows0)(),
		int (*tc_isMac0)(),
		int (*tc_isLinux0)(),
		String (*tc_appDir0)(),
		
		void (*tc_createInputWindow0)(TableOfReals,String,String, String),
        void (*tc_createInputWindow1)(TableOfReals, String, void (*f)(TableOfReals)),
		void (*createSliders)(Item, TableOfReals, void (*f)(TableOfReals)),
		
		void (*tc_addInputWindowOptions0)(String, int i, int j, ArrayOfStrings),
		void (*tc_addInputWindowCheckbox0)(String, int i, int j),
		void (*tc_openNewWindow0)(String title),
		
		ArrayOfItems (*tc_getChildren0)(Item),
		Item (*tc_getParent0)(Item),
		
		TableOfReals (*tc_getNumericalData0)(Item,String),
		void (*tc_setNumericalData0)(Item,String,TableOfReals),
		TableOfStrings (*tc_getTextData0)(Item,String),
		void (*tc_setTextData0)(Item,String, TableOfStrings),
				
		ArrayOfStrings (*tc_getNumericalDataNames0)(Item),
		ArrayOfStrings (*tc_getTextDataNames0)(Item),
		
		void (*tc_zoom0)(double factor),
		
		String (*getString)(String),
		int (*getSelectedString)(String, ArrayOfStrings, String),
		double (*getNumber)(String),
		void (*getNumbers)( ArrayOfStrings, double * ),
		String (*getFilename)(),
		
		int (*askQuestion)(String),
		void (*messageDialog)(String),
		
		void (*setSize0)(void*,double,double,int),
		double (*getWidth0)(void*),
		double (*getHeight0)(void*),
		void (*setAngle0)(void*,double,int),
		double (*getAngle0)(void*),
		String (*getColor)(void*),
		void (*setColor0)(void*,String,int),
		
		void (*changeGraphics0)(void*,String),
		void (*changeArrowHead0)(void*,String)
	);

/*! 
 \brief show progress of current operation
 \ingroup Input and Output
*/
TCAPIEXPORT void tc_showProgress(int progress);

/*! 
 \brief initialize main
 \ingroup init
*/
TCAPIEXPORT void tc_CThread_api_initialize( 
	Item cthread,
	void (*showProgress)(Item, int)	);

#endif
