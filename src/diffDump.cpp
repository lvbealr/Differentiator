#include "differentiator.h"
#include "diffDump.h"

DiffError diffTreeDump(binaryTree<DiffNode> *tree) {
  customWarning(tree != NULL, TREE_ERROR);

  tree->infoData->dumpFileName = "temp.dot";

  FILE *dumpFile = fopen(tree->infoData->dumpFileName, "w");
  customWarning(dumpFile != NULL, DUMP_FILE_ERROR);

  // .DOT HEADER //
  fprintf(dumpFile, "digraph diffTree {\nsplines=polyline;\nrankdir=HR;\nnodesep=0.4;"
                    "\nnode [shape=record, fontname=\"JetBrains Mono\", fontsize=\"10\", color=\"gray\", style=\"rounded\"];\n"
                    "edge [style=dashed, color=\"green\", weight=\"10\", penwidth=\"2\", "
                    "arrowsize=\"0.4\"];\n");
  // .DOT HEADER //

  // NODE //
  if (tree->root) {
    diffTreeNodeDump    (dumpFile, tree->root);
    diffTreeNodeDumpLink(dumpFile, tree->root);
  }
  // NODE //

  fprintf(dumpFile, "}\n");

  char *buffer = (char *)calloc(MAX_CMD_BUFFER_SIZE, sizeof(char));
  customWarning(buffer != NULL, BUFFER_ERROR);

  fclose(dumpFile);

  snprintf(buffer, MAX_CMD_BUFFER_SIZE, "echo '<div>' >> %s", tree->infoData->htmlDumpPath);
  system(buffer);

  *buffer = {};

  writeHtmlHeader(tree);

  snprintf(buffer, MAX_CMD_BUFFER_SIZE, "dot -Tsvg %s >> %s",
          tree->infoData->dumpFileName, tree->infoData->htmlDumpPath);
  system(buffer);

  *buffer = {};

  snprintf(buffer, MAX_CMD_BUFFER_SIZE, "echo '</div><hr size='2' color='#ff9900'>' >> %s", tree->infoData->htmlDumpPath);
  system(buffer);

  *buffer = {};

  snprintf(buffer, MAX_CMD_BUFFER_SIZE, "rm -rf temp.svg");
  system(buffer);

  FREE_(buffer);

  return NO_DIFF_ERRORS;
}

DiffError diffTreeNodeDump      (FILE *dumpFile, node<DiffNode> *node) {
  customWarning(dumpFile != NULL, DUMP_FILE_ERROR);
  customWarning(node     != NULL, NODE_NULL_PTR);

  switch (node->data.type) {
    case NUMERICAL_NODE:
      {
        fprintf(dumpFile, "p%p [label = \"{ <p> [%p] | <t> type = NUMERICAL_NODE [%d] | <d> data = [%lg] | { <l> [%p] | <r> [%p] }}\","
                          "color = \"green\"];\n",
          node, node, node->data.type, node->data.nodeValue.value, node->left, node->right);
        break;
      }

    #define NAME_OPERATION(operationType) {                                                                                              \
      case operationType:                                                                                                                \
        {                                                                                                                                \
          fprintf(dumpFile, "p%p [label = \"{ <p> [%p] | <t> type = OPERATION_NODE [%d] | <d> data = [%s] | { <l> [%p] | <r> [%p] }}\"," \
                            "color = \"purple\"];\n",                                                                                    \
                            node, node, node->data.type, #operationType, node->left, node->right);                                       \
          break;                                                                                                                         \
        }                                                                                                                                \
      }

    case OPERATION_NODE:
      {
        switch (node->data.nodeValue.op.name) {
          NAME_OPERATION (ADD);
          NAME_OPERATION (SUB);
          NAME_OPERATION (MUL);
          NAME_OPERATION (DIV);
          NAME_OPERATION (POW);
          NAME_OPERATION (SIN);
          NAME_OPERATION (COS);
          NAME_OPERATION (EXP);
          NAME_OPERATION  (LN);
          NAME_OPERATION(SQRT);
        }

        break;
      }

    #undef NAME_OPERATION

    case VARIABLE_NODE:
      {
        fprintf(dumpFile, "p%p [label = \"{ <p> [%p] | <t> type = VARIABLE_NODE [%d] | <d> data = [%c] | { <l> [%p] | <r> [%p] }}\","
                          "color = \"blue\"];\n",
          node, node, node->data.type, node->data.nodeValue.varIndex, node->left, node->right);
        break;
      }

    default:
      {
        break;
      }
  }

  if (node->left != NULL) {
      diffTreeNodeDump(dumpFile, node->left);
  }

  if (node->right != NULL) {
      diffTreeNodeDump(dumpFile, node->right);
  }

  return NO_DIFF_ERRORS;
}

DiffError diffTreeNodeDumpLink(FILE *dumpFile, node<DiffNode> *node) {
  customWarning(dumpFile != NULL, DUMP_FILE_ERROR);
  customWarning(node     != NULL, NODE_NULL_PTR);

  if (node->left != NULL) {
    fprintf(dumpFile, "p%p:<l> -> p%p\n", node, node->left);
    diffTreeNodeDumpLink(dumpFile, node->left);
  }

  if (node->right != NULL) {
    fprintf(dumpFile, "p%p:<r> -> p%p\n", node, node->right);
    diffTreeNodeDumpLink(dumpFile, node->right);
  }

  return NO_DIFF_ERRORS;
}


DiffError writeHtmlHeader(binaryTree<DiffNode> *tree) {
  customWarning(tree != NULL, TREE_ERROR);

  char *header = (char *)calloc(MAX_HEADER_SIZE, sizeof(char));
  customWarning(header != NULL, DUMP_FILE_ERROR);

  snprintf(header, MAX_HEADER_SIZE, "<br><br><div style='font-size:22px'><b><u>binaryTree</u><font color='DeepSkyBlue'>"
                                    " [%p]" "</font></b>"
                                    " at <b><u>%s:%d</u> <u>(%s)</u></b> <font color='DarkOrange'><b><br>born at</b></font>"
                                    " <b><u>%s:%d</u></b> (%s)<br><br></div>",
          tree, tree->infoData->lastUsedFileName, tree->infoData->lastUsedLine, tree->infoData->lastUsedFunctionName,
                tree->infoData->bornFileName,     tree->infoData->bornLine,     tree->infoData->bornFunctionName);

  int openFile = open(tree->infoData->htmlDumpPath, O_WRONLY | O_APPEND);
  customWarning(openFile != -1, DUMP_FILE_ERROR);

  ssize_t writeFile = write(openFile, header, MAX_HEADER_SIZE);

  close(openFile);

  FREE_(header);

  return NO_DIFF_ERRORS;
}