#include <cstdio>

#include "differentiator.h"

int main(int argc, char *argv[]) {
  Differentiator Diff = {};
  treeInitialize(&Diff.diffTree);

  Diff.diffTree.root->data.type            = NUMERICAL_NODE;
  Diff.diffTree.root->data.nodeValue.value = 228;

  printf("%lg", Diff.diffTree.root->data.nodeValue.value);

  return 0;
}