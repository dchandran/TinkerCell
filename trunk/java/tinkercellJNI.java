/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.40
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */


class tinkercellJNI {
  public final static native void tc_strings_length_set(long jarg1, tc_strings jarg1_, int jarg2);
  public final static native int tc_strings_length_get(long jarg1, tc_strings jarg1_);
  public final static native void tc_strings_strings_set(long jarg1, tc_strings jarg1_, long jarg2);
  public final static native long tc_strings_strings_get(long jarg1, tc_strings jarg1_);
  public final static native long new_tc_strings();
  public final static native void delete_tc_strings(long jarg1);
  public final static native void tc_items_length_set(long jarg1, tc_items jarg1_, int jarg2);
  public final static native int tc_items_length_get(long jarg1, tc_items jarg1_);
  public final static native void tc_items_items_set(long jarg1, tc_items jarg1_, long jarg2);
  public final static native long tc_items_items_get(long jarg1, tc_items jarg1_);
  public final static native long new_tc_items();
  public final static native void delete_tc_items(long jarg1);
  public final static native void tc_matrix_rows_set(long jarg1, tc_matrix jarg1_, int jarg2);
  public final static native int tc_matrix_rows_get(long jarg1, tc_matrix jarg1_);
  public final static native void tc_matrix_cols_set(long jarg1, tc_matrix jarg1_, int jarg2);
  public final static native int tc_matrix_cols_get(long jarg1, tc_matrix jarg1_);
  public final static native void tc_matrix_values_set(long jarg1, tc_matrix jarg1_, long jarg2);
  public final static native long tc_matrix_values_get(long jarg1, tc_matrix jarg1_);
  public final static native void tc_matrix_rownames_set(long jarg1, tc_matrix jarg1_, long jarg2, tc_strings jarg2_);
  public final static native long tc_matrix_rownames_get(long jarg1, tc_matrix jarg1_);
  public final static native void tc_matrix_colnames_set(long jarg1, tc_matrix jarg1_, long jarg2, tc_strings jarg2_);
  public final static native long tc_matrix_colnames_get(long jarg1, tc_matrix jarg1_);
  public final static native long new_tc_matrix();
  public final static native void delete_tc_matrix(long jarg1);
  public final static native void tc_table_rows_set(long jarg1, tc_table jarg1_, int jarg2);
  public final static native int tc_table_rows_get(long jarg1, tc_table jarg1_);
  public final static native void tc_table_cols_set(long jarg1, tc_table jarg1_, int jarg2);
  public final static native int tc_table_cols_get(long jarg1, tc_table jarg1_);
  public final static native void tc_table_strings_set(long jarg1, tc_table jarg1_, long jarg2);
  public final static native long tc_table_strings_get(long jarg1, tc_table jarg1_);
  public final static native void tc_table_rownames_set(long jarg1, tc_table jarg1_, long jarg2, tc_strings jarg2_);
  public final static native long tc_table_rownames_get(long jarg1, tc_table jarg1_);
  public final static native void tc_table_colnames_set(long jarg1, tc_table jarg1_, long jarg2, tc_strings jarg2_);
  public final static native long tc_table_colnames_get(long jarg1, tc_table jarg1_);
  public final static native long new_tc_table();
  public final static native void delete_tc_table(long jarg1);
  public final static native long tc_createMatrix(int jarg1, int jarg2);
  public final static native long tc_createTable(int jarg1, int jarg2);
  public final static native long tc_createStringsArray(int jarg1);
  public final static native long tc_createItemsArray(int jarg1);
  public final static native double tc_getMatrixValue(long jarg1, tc_matrix jarg1_, int jarg2, int jarg3);
  public final static native void tc_setMatrixValue(long jarg1, tc_matrix jarg1_, int jarg2, int jarg3, double jarg4);
  public final static native String tc_getRowName(long jarg1, tc_matrix jarg1_, int jarg2);
  public final static native void tc_setRowName(long jarg1, tc_matrix jarg1_, int jarg2, String jarg3);
  public final static native String tc_getColumnName(long jarg1, tc_matrix jarg1_, int jarg2);
  public final static native void tc_setColumnName(long jarg1, tc_matrix jarg1_, int jarg2, String jarg3);
  public final static native String tc_getTableValue(long jarg1, tc_table jarg1_, int jarg2, int jarg3);
  public final static native void tc_setTableValue(long jarg1, tc_table jarg1_, int jarg2, int jarg3, String jarg4);
  public final static native String tc_getString(long jarg1, tc_strings jarg1_, int jarg2);
  public final static native void tc_setString(long jarg1, tc_strings jarg1_, int jarg2, String jarg3);
  public final static native int tc_getItem(long jarg1, tc_items jarg1_, int jarg2);
  public final static native void tc_setItem(long jarg1, tc_items jarg1_, int jarg2, int jarg3);
  public final static native void tc_deleteMatrix(long jarg1, tc_matrix jarg1_);
  public final static native void tc_deleteTable(long jarg1, tc_table jarg1_);
  public final static native void tc_deleteItemsArray(long jarg1, tc_items jarg1_);
  public final static native void tc_deleteStringsArray(long jarg1, tc_strings jarg1_);
  public final static native long tc_appendColumns(long jarg1, tc_matrix jarg1_, long jarg2, tc_matrix jarg2_);
  public final static native long tc_appendRows(long jarg1, tc_matrix jarg1_, long jarg2, tc_matrix jarg2_);
  public final static native void tc_printMatrixToFile(String jarg1, long jarg2, tc_matrix jarg2_);
  public final static native void tc_printOutMatrix(long jarg1, tc_matrix jarg1_);
  public final static native void tc_printTableToFile(String jarg1, long jarg2, tc_table jarg2_);
  public final static native void tc_printOutTable(long jarg1, tc_table jarg1_);
  public final static native long tc_allItems();
  public final static native long tc_selectedItems();
  public final static native long tc_itemsOfFamily(String jarg1);
  public final static native long tc_itemsOfFamilyFrom(String jarg1, long jarg2, tc_items jarg2_);
  public final static native int tc_find(String jarg1);
  public final static native long tc_findItems(long jarg1, tc_strings jarg1_);
  public final static native void tc_select(int jarg1);
  public final static native void tc_deselect();
  public final static native String tc_getName(int jarg1);
  public final static native String tc_getUniqueName(int jarg1);
  public final static native void tc_rename(int jarg1, String jarg2);
  public final static native long tc_getNames(long jarg1, tc_items jarg1_);
  public final static native long tc_getUniqueNames(long jarg1, tc_items jarg1_);
  public final static native String tc_getFamily(int jarg1);
  public final static native int tc_isA(int jarg1, String jarg2);
  public final static native void tc_print(String jarg1);
  public final static native void tc_errorReport(String jarg1);
  public final static native void tc_printMatrix(long jarg1, tc_matrix jarg1_);
  public final static native void tc_printFile(String jarg1);
  public final static native void tc_clear();
  public final static native void tc_remove(int jarg1);
  public final static native double tc_getY(int jarg1);
  public final static native double tc_getX(int jarg1);
  public final static native long tc_getPos(long jarg1, tc_items jarg1_);
  public final static native void tc_setPos(int jarg1, double jarg2, double jarg3);
  public final static native void tc_setPosMulti(long jarg1, tc_items jarg1_, long jarg2, tc_matrix jarg2_);
  public final static native void tc_moveSelected(double jarg1, double jarg2);
  public final static native int tc_isWindows();
  public final static native int tc_isMac();
  public final static native int tc_isLinux();
  public final static native String tc_appDir();
  public final static native String tc_homeDir();
  public final static native void tc_createInputWindowForScript(long jarg1, tc_matrix jarg1_, String jarg2, String jarg3);
  public final static native void tc_createInputWindow(long jarg1, tc_matrix jarg1_, String jarg2, long jarg3);
  public final static native void tc_addInputWindowOptions(String jarg1, int jarg2, int jarg3, long jarg4, tc_strings jarg4_);
  public final static native void tc_addInputWindowCheckbox(String jarg1, int jarg2, int jarg3);
  public final static native void tc_openNewWindow(String jarg1);
  public final static native long tc_getChildren(int jarg1);
  public final static native int tc_getParent(int jarg1);
  public final static native long tc_getNumericalData(int jarg1, String jarg2);
  public final static native void tc_setNumericalData(int jarg1, String jarg2, long jarg3, tc_matrix jarg3_);
  public final static native void tc_setNumericalValues(long jarg1, tc_matrix jarg1_);
  public final static native void tc_setNumericalValue(String jarg1, double jarg2);
  public final static native long tc_getTextData(int jarg1, String jarg2);
  public final static native void tc_setTextData(int jarg1, String jarg2, long jarg3, tc_table jarg3_);
  public final static native void tc_setTextValues(long jarg1, tc_table jarg1_);
  public final static native double tc_getNumericalValue(String jarg1);
  public final static native String tc_getTextValue(String jarg1);
  public final static native void tc_setTextValue(String jarg1, String jarg2);
  public final static native long tc_getNumericalDataNames(int jarg1);
  public final static native long tc_getTextDataNames(int jarg1);
  public final static native void tc_zoom(double jarg1);
  public final static native String tc_getStringDialog(String jarg1);
  public final static native String tc_getFilename();
  public final static native int tc_getStringFromList(String jarg1, long jarg2, tc_strings jarg2_, String jarg3);
  public final static native double tc_getNumber(String jarg1);
  public final static native void tc_getNumbers(long jarg1, tc_strings jarg1_, long jarg2);
  public final static native int tc_askQuestion(String jarg1);
  public final static native void tc_messageDialog(String jarg1);
  public final static native void tc_openFile(String jarg1);
  public final static native void tc_saveToFile(String jarg1);
  public final static native int tc_thisThread();
  public final static native void tc_createSliders(long jarg1, tc_matrix jarg1_, long jarg2);
  public final static native String tc_getColor(int jarg1);
  public final static native void tc_setColor(int jarg1, String jarg2, int jarg3);
  public final static native void tc_changeNodeImage(int jarg1, String jarg2);
  public final static native void tc_changeArrowHead(int jarg1, String jarg2);
  public final static native void tc_setSize(int jarg1, double jarg2, double jarg3);
  public final static native double tc_getWidth(int jarg1);
  public final static native double tc_getHeight(int jarg1);
  public final static native void tc_rotate(int jarg1, double jarg2);
  public final static native void tc_screenshot(String jarg1, int jarg2, int jarg3);
  public final static native int tc_screenWidth();
  public final static native int tc_screenHeight();
  public final static native int tc_screenX();
  public final static native int tc_screenY();
  public final static native String tc_annotations();
  public final static native void tc_insertAnnotations(String jarg1, double jarg2, double jarg3);
  public final static native void tc_Main_api_initialize(long jarg1, long jarg2, long jarg3, long jarg4, long jarg5, long jarg6, long jarg7, long jarg8, long jarg9, long jarg10, long jarg11, long jarg12, long jarg13, long jarg14, long jarg15, long jarg16, long jarg17, long jarg18, long jarg19, long jarg20, long jarg21, long jarg22, long jarg23, long jarg24, long jarg25, long jarg26, long jarg27, long jarg28, long jarg29, long jarg30, long jarg31, long jarg32, long jarg33, long jarg34, long jarg35, long jarg36, long jarg37, long jarg38, long jarg39, long jarg40, long jarg41, long jarg42, long jarg43, long jarg44, long jarg45, long jarg46, long jarg47, long jarg48, long jarg49, long jarg50, long jarg51, long jarg52, long jarg53, long jarg54, long jarg55, long jarg56, long jarg57, long jarg58, long jarg59, long jarg60, long jarg61, long jarg62, long jarg63, long jarg64, long jarg65, long jarg66, long jarg67, long jarg68, long jarg69, long jarg70, long jarg71, long jarg72, long jarg73, long jarg74, long jarg75, long jarg76, long jarg77);
  public final static native void tc_showProgress(int jarg1);
  public final static native void tc_setProgessBarTitle(String jarg1);
  public final static native void tc_callback(long jarg1);
  public final static native void tc_callWhenExiting(long jarg1);
  public final static native void tc_CThread_api_initialize(int jarg1, long jarg2, long jarg3, long jarg4, long jarg5);
  public final static native long tc_getParameters(long jarg1, tc_items jarg1_);
  public final static native long tc_getInitialValues(long jarg1, tc_items jarg1_);
  public final static native void tc_setInitialValues(long jarg1, tc_items jarg1_, long jarg2, tc_matrix jarg2_);
  public final static native long tc_getFixedVariables(long jarg1, tc_items jarg1_);
  public final static native long tc_getParametersAndFixedVariables(long jarg1, tc_items jarg1_);
  public final static native String tc_getTextAttribute(int jarg1, String jarg2);
  public final static native double tc_getParameter(int jarg1, String jarg2);
  public final static native long tc_getParametersNamed(long jarg1, tc_items jarg1_, long jarg2, tc_strings jarg2_);
  public final static native long tc_getParametersExcept(long jarg1, tc_items jarg1_, long jarg2, tc_strings jarg2_);
  public final static native long tc_getAllTextNamed(long jarg1, tc_items jarg1_, long jarg2, tc_strings jarg2_);
  public final static native void tc_setTextAttribute(int jarg1, String jarg2, String jarg3);
  public final static native void tc_setParameter(int jarg1, String jarg2, double jarg3);
  public final static native void tc_setTextAttributeByName(String jarg1, String jarg2);
  public final static native void tc_setParameterByName(String jarg1, double jarg2);
  public final static native void tc_setTextAttributes(long jarg1, tc_table jarg1_);
  public final static native void tc_setParameters(long jarg1, tc_matrix jarg1_);
  public final static native void tc_BasicInformationTool_Text_api(long jarg1, long jarg2, long jarg3);
  public final static native void tc_BasicInformationTool_Numeric_api(long jarg1, long jarg2, long jarg3, long jarg4, long jarg5, long jarg6, long jarg7, long jarg8, long jarg9);
  public final static native int tc_insertConnection(long jarg1, tc_items jarg1_, String jarg2, String jarg3);
  public final static native long tc_getConnectedNodes(int jarg1);
  public final static native long tc_getConnectedNodesWithRole(int jarg1, String jarg2);
  public final static native long tc_getConnections(int jarg1);
  public final static native long tc_getConnectionsWithRole(int jarg1, String jarg2);
  public final static native void tc_ConnectionInsertion_api(long jarg1, long jarg2, long jarg3, long jarg4, long jarg5);
  public final static native double tc_getControlPointX(int jarg1, int jarg2, int jarg3);
  public final static native double tc_getControlPointY(int jarg1, int jarg2, int jarg3);
  public final static native void tc_setControlPoint(int jarg1, int jarg2, int jarg3, double jarg4, double jarg5);
  public final static native void tc_setCenterPoint(int jarg1, double jarg2, double jarg3);
  public final static native double tc_getCenterPointX(int jarg1);
  public final static native double tc_getCenterPointY(int jarg1);
  public final static native void tc_setStraight(int jarg1, int jarg2);
  public final static native void tc_setAllStraight(int jarg1);
  public final static native void tc_setLineWidth(int jarg1, double jarg2, int jarg3);
  public final static native void tc_ConnectionSelection_api(long jarg1, long jarg2, long jarg3, long jarg4, long jarg5, long jarg6, long jarg7, long jarg8, long jarg9);
  public final static native void tc_merge(long jarg1, tc_items jarg1_);
  public final static native void tc_separate(int jarg1);
  public final static native void tc_GroupHandlerTool_api(long jarg1, long jarg2);
  public final static native int tc_insert(String jarg1, String jarg2);
  public final static native void tc_NodeInsertion_api(long jarg1);
  public final static native long tc_getStoichiometry(long jarg1, tc_items jarg1_);
  public final static native void tc_setStoichiometry(long jarg1, tc_items jarg1_, long jarg2, tc_matrix jarg2_);
  public final static native long tc_getRates(long jarg1, tc_items jarg1_);
  public final static native void tc_setRates(long jarg1, tc_items jarg1_, long jarg2, tc_strings jarg2_);
  public final static native long tc_getStoichiometryFor(int jarg1);
  public final static native String tc_getRate(int jarg1);
  public final static native void tc_setRate(int jarg1, String jarg2);
  public final static native void tc_setStoichiometryFor(int jarg1, long jarg2, tc_matrix jarg2_);
  public final static native void tc_StoichiometryTool_api(long jarg1, long jarg2, long jarg3, long jarg4);
  public final static native int tc_compileAndRun(String jarg1, String jarg2);
  public final static native int tc_compileBuildLoad(String jarg1, String jarg2, String jarg3);
  public final static native int tc_compileBuildLoadSliders(String jarg1, String jarg2, String jarg3, long jarg4, tc_matrix jarg4_);
  public final static native void tc_runPythonCode(String jarg1);
  public final static native void tc_runPythonFile(String jarg1);
  public final static native void tc_addPythonPlugin(String jarg1, String jarg2, String jarg3, String jarg4, String jarg5);
  public final static native void tc_callFunction(String jarg1);
  public final static native void tc_loadLibrary(String jarg1);
  public final static native void tc_OctaveTool_api(long jarg1, long jarg2, long jarg3);
  public final static native void tc_addFunction(long jarg1, String jarg2, String jarg3, String jarg4, String jarg5, String jarg6, int jarg7, int jarg8, int jarg9);
  public final static native void tc_runOctaveCode(String jarg1);
  public final static native void tc_runOctaveFile(String jarg1);
  public final static native void tc_addOctavePlugin(String jarg1, String jarg2, String jarg3, String jarg4, String jarg5);
  public final static native void tc_DynamicLibraryMenu_api(long jarg1);
  public final static native void tc_LoadCLibraries_api(long jarg1, long jarg2, long jarg3, long jarg4, long jarg5);
  public final static native void tc_PythonTool_api(long jarg1, long jarg2, long jarg3);
  public final static native void tc_surface(long jarg1, tc_matrix jarg1_, String jarg2);
  public final static native void tc_plot(long jarg1, tc_matrix jarg1_, String jarg2);
  public final static native void tc_scatterplot(long jarg1, tc_matrix jarg1_, String jarg2);
  public final static native void tc_errorBars(long jarg1, tc_matrix jarg1_, String jarg2);
  public final static native void tc_hist(long jarg1, tc_matrix jarg1_, String jarg2);
  public final static native void tc_multiplot(int jarg1, int jarg2);
  public final static native void tc_holdPlot(int jarg1);
  public final static native void tc_clusterPlots(int jarg1);
  public final static native long tc_getPlotData(int jarg1);
  public final static native void tc_gnuplot(String jarg1);
  public final static native void tc_savePlot(String jarg1);
  public final static native void tc_setLogScale(int jarg1);
  public final static native void tc_PlotTool_api(long jarg1, long jarg2, long jarg3, long jarg4, long jarg5, long jarg6, long jarg7, long jarg8, long jarg9, long jarg10, long jarg11, long jarg12);
  public final static native int tc_writeModel(String jarg1, long jarg2, tc_items jarg2_);
  public final static native void tc_ModelFileGenerator_api(long jarg1);
  public final static native long tc_getEventTriggers();
  public final static native long tc_getEventResponses();
  public final static native void tc_addEvent(String jarg1, String jarg2);
  public final static native void tc_SimulationEventsTool_api(long jarg1, long jarg2, long jarg3);
  public final static native long tc_getForcingFunctionNames(long jarg1, tc_items jarg1_);
  public final static native long tc_getForcingFunctionAssignments(long jarg1, tc_items jarg1_);
  public final static native void tc_addForcingFunction(int jarg1, String jarg2, String jarg3);
  public final static native void tc_AssignmentFunctionsTool_api(long jarg1, long jarg2, long jarg3);
  public final static native void tc_displayText(int jarg1, String jarg2);
  public final static native void tc_displayNumber(int jarg1, double jarg2);
  public final static native void tc_setDisplayLabelColor(String jarg1, String jarg2);
  public final static native void tc_highlight(int jarg1, String jarg2);
  public final static native void tc_CLabelsTool_api(long jarg1, long jarg2, long jarg3, long jarg4);
  public final static native long tc_getAnnotation(int jarg1);
  public final static native void tc_setAnnotation(int jarg1, long jarg2, tc_strings jarg2_);
  public final static native void tc_NameFamily_api_initialize(long jarg1, long jarg2);
  public final static native long tc_partsIn(int jarg1);
  public final static native long tc_partsUpstream(int jarg1);
  public final static native long tc_partsDownstream(int jarg1);
  public final static native void tc_alignParts(long jarg1, tc_items jarg1_);
  public final static native void tc_alignPartsOnPlasmid(int jarg1, long jarg2, tc_items jarg2_);
  public final static native void tc_setSequence(int jarg1, String jarg2);
  public final static native void tc_AutoGeneRegulatoryTool_api(long jarg1, long jarg2, long jarg3, long jarg4, long jarg5);
  public final static native void tc_exportSBML(String jarg1);
  public final static native void tc_importSBML(String jarg1);
  public final static native void tc_SBML_api(long jarg1, long jarg2);
  public final static native long tc_simulateDeterministic(double jarg1, double jarg2, int jarg3);
  public final static native long tc_simulateStochastic(double jarg1, double jarg2, int jarg3);
  public final static native long tc_simulateHybrid(double jarg1, double jarg2, int jarg3);
  public final static native long tc_simulateTauLeap(double jarg1, double jarg2, int jarg3);
  public final static native long tc_getSteadyState();
  public final static native long tc_steadyStateScan(String jarg1, double jarg2, double jarg3, int jarg4);
  public final static native long tc_steadyStateScan2D(String jarg1, double jarg2, double jarg3, int jarg4, String jarg5, double jarg6, double jarg7, int jarg8);
  public final static native long tc_getJacobian();
  public final static native long tc_getEigenvalues();
  public final static native long tc_getUnscaledElasticities();
  public final static native long tc_getUnscaledConcentrationCC();
  public final static native long tc_getUnscaledFluxCC();
  public final static native long tc_getScaledElasticities();
  public final static native long tc_getScaledConcentrationCC();
  public final static native long tc_getScaledFluxCC();
  public final static native long tc_reducedStoichiometry();
  public final static native long tc_elementaryFluxModes();
  public final static native long tc_LMatrix();
  public final static native long tc_KMatrix();
  public final static native long tc_optimize(String jarg1);
  public final static native void tc_COPASI_api(long jarg1, long jarg2, long jarg3, long jarg4, long jarg5, long jarg6, long jarg7, long jarg8, long jarg9, long jarg10, long jarg11, long jarg12, long jarg13, long jarg14, long jarg15, long jarg16, long jarg17, long jarg18, long jarg19, long jarg20);
  public final static native void tc_substituteModel(int jarg1, String jarg2);
  public final static native void tc_substituteEmptyModel(int jarg1);
  public final static native void tc_substituteOriginalModel(int jarg1);
  public final static native long tc_listOfPossibleModels(int jarg1);
  public final static native void tc_ModuleTool_api(long jarg1, long jarg2);
}
