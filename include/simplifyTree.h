#ifndef SIMPLIFY_TREE_H_
#define SIMPLIFY_TREE_H_

enum simplifyError {
    NO_SIMPLIFY_ERRORS              = 0,
    SIMPLIFICATIONS_NUMBER_NULL_PTR = 1
};

enum zeroComparisonCode {
    DOUBLE_LESS_EPS    = 0,
    DOUBLE_EQUAL_EPS   = 1,
    DOUBLE_GREATER_EPS = 2
};

const double EPS = 1e-9;

// FUNCTION PROTOTYPES //
zeroComparisonCode zeroComparison(double x);

binaryTreeError destructSubtree(node<diffNode> **formerNode);
simplifyError   replaceNode    (Differentiator *diff, node<diffNode> **formerNode, node<diffNode> *futureNode);

simplifyError simplifyTree   (Differentiator *diff);
simplifyError simplifySubtree(Differentiator *diff, node<diffNode> **currentNode, size_t *simplificationsNumber);
// FUNCTION PROTOTYPES //

#endif // SIMPLIFY_TREE_H_
