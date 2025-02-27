SET(LIBRARY_OUTPUT_PATH ${CPP_LIBRARY_OUTPUT_PATH})
 INCLUDE_DIRECTORIES( BEFORE
    ${TINKERCELL_SOURCE_DIR}/BasicTools
    ${TINKERCELL_SOURCE_DIR}/GeneticNetworks
    ${TINKERCELL_SOURCE_DIR}/NodesTree
    ${TINKERCELL_SOURCE_DIR}/ModelingTools
 )
 
ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_PLUGIN)
ADD_DEFINITIONS(-DQT_NO_DEBUG)
ADD_DEFINITIONS(-DQT_SHARED)

SET( TINKERCELL_CORE_HDRS
  ../Core/MainWindow.h 
  ../Core/NetworkHandle.h
  ../Core/NetworkWindow.h
  ../Core/CodeEditor.h
  ../Core/TextEditor.h 
  ../Core/GraphicsScene.h
  ../Core/GraphicsView.h  
  ../Core/Tool.h
  )

QT4_WRAP_CPP( TinkercellCore_MOC ${TINKERCELL_CORE_HDRS})

QT4_WRAP_CPP( AutoGeneRegulatoryTool_MOC AutoGeneRegulatoryTool.h)

ADD_LIBRARY( AutoGeneRegulatoryTool
  SHARED
  AutoGeneRegulatoryTool.cpp
  ${AutoGeneRegulatoryTool_MOC}
  ${TinkercellCore_MOC}
  )

TARGET_LINK_LIBRARIES( AutoGeneRegulatoryTool
  AutoGeneRegulatoryTool
  NodesTree
  TinkerCellCore
)

IF ( WIN32 )
  INSTALL(TARGETS AutoGeneRegulatoryTool
    DESTINATION plugins
  )
ELSE ( WIN32 )
  INSTALL(TARGETS AutoGeneRegulatoryTool
    LIBRARY DESTINATION plugins
  )
ENDIF( WIN32 )


QT4_WRAP_CPP( DNASequenceTool_MOC DNASequenceTool.h CLabelsTool.h)

ADD_LIBRARY( DNASequenceViewer 
  SHARED
  CLabelsTool.cpp
  DNASequenceTool.cpp
  ${DNASequenceTool_MOC}
  ${TinkercellCore_MOC}
  )

TARGET_LINK_LIBRARIES( DNASequenceViewer
  DNASequenceViewer
  TinkerCellCore
  AutoGeneRegulatoryTool
)

IF ( WIN32 )
  INSTALL(TARGETS DNASequenceViewer
    DESTINATION plugins
  )
ELSE ( WIN32 )
  INSTALL(TARGETS DNASequenceViewer
    LIBRARY DESTINATION plugins
  )
ENDIF( WIN32 )

