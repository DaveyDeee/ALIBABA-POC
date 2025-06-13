#include <bits/stdc++.h>
using namespace std;

const int MaxCapacity = 2000005;

// This class finds the number of rectangles "flagged" by the segment tree sweep
class RectangleCoverageSolver {
    // Coordinate compression arrays (0th index stores the count)
    int compressedX[MaxCapacity], compressedY[MaxCapacity];
    // Removal flags
    bool isRemoved[MaxCapacity], isFlagged[MaxCapacity];

    // Rectangle: leftX, leftY, rightX, rightY (all compressed)
    struct Rectangle { int leftX, leftY, rightX, rightY; };
    Rectangle rectangles[MaxCapacity];

    // Events for segment tree sweep: fromY, toY, rectangleIndex
    struct YAxisEvent { int fromY, toY, rectangleIndex; };
    vector<YAxisEvent> eventsAddAtX[MaxCapacity], eventsRemoveAtX[MaxCapacity];

    // Segment tree node structure
    struct SegmentTreeNode {
        int minActiveValue, maxActiveValue, pendingValue, cleanupThreshold;
        priority_queue<int> activeRectangles, cleanupCandidates;
    };
    SegmentTreeNode segmentTree[MaxCapacity * 4];

    // Builds the segment tree over [leftIndex, rightIndex]
    void buildSegmentTree(int nodeIndex, int leftIndex, int rightIndex) {
        segmentTree[nodeIndex].activeRectangles.push(0);
        segmentTree[nodeIndex].cleanupCandidates.push(0);
        if (leftIndex == rightIndex) return;
        int midIndex = (leftIndex + rightIndex) / 2;
        buildSegmentTree(nodeIndex * 2, leftIndex, midIndex);
        buildSegmentTree(nodeIndex * 2 + 1, midIndex + 1, rightIndex);
    }

    // Updates parent node after modifications in children or self
    void updateSegmentTree(int nodeIndex, int leftIndex, int rightIndex) {
        if (leftIndex != rightIndex) {
            segmentTree[nodeIndex].minActiveValue = max(
                min(segmentTree[nodeIndex * 2].minActiveValue, segmentTree[nodeIndex * 2 + 1].minActiveValue),
                segmentTree[nodeIndex].maxActiveValue
            );
            segmentTree[nodeIndex].cleanupThreshold = max(
                max(segmentTree[nodeIndex * 2].cleanupThreshold, segmentTree[nodeIndex * 2 + 1].cleanupThreshold),
                segmentTree[nodeIndex].pendingValue
            );
        } else {
            segmentTree[nodeIndex].minActiveValue = segmentTree[nodeIndex].maxActiveValue;
            segmentTree[nodeIndex].cleanupThreshold = segmentTree[nodeIndex].pendingValue;
        }
        // If the threshold is below min, reset it
        if (segmentTree[nodeIndex].cleanupThreshold < segmentTree[nodeIndex].minActiveValue)
            segmentTree[nodeIndex].cleanupThreshold = 0;
    }

    // Adds a new rectangle to the current segment tree node
    void addRectangleToNode(int nodeIndex, int rectangleId, int currentMaxValue) {
        if (rectangleId >= max(currentMaxValue, segmentTree[nodeIndex].minActiveValue)) {
            isFlagged[rectangleId] = true;
        } else {
            segmentTree[nodeIndex].pendingValue = max(segmentTree[nodeIndex].pendingValue, rectangleId);
        }
        segmentTree[nodeIndex].maxActiveValue = max(segmentTree[nodeIndex].maxActiveValue, rectangleId);
        segmentTree[nodeIndex].activeRectangles.push(rectangleId);
        segmentTree[nodeIndex].cleanupCandidates.push(rectangleId);
    }

    // Removes a rectangle from the current segment tree node
    void removeRectangleFromNode(int nodeIndex, int rectangleId, int currentMaxValue) {
        while (!segmentTree[nodeIndex].activeRectangles.empty() && isRemoved[segmentTree[nodeIndex].activeRectangles.top()])
            segmentTree[nodeIndex].activeRectangles.pop();
        while (!segmentTree[nodeIndex].cleanupCandidates.empty() &&
                (isRemoved[segmentTree[nodeIndex].cleanupCandidates.top()] ||
                 isFlagged[segmentTree[nodeIndex].cleanupCandidates.top()]))
            segmentTree[nodeIndex].cleanupCandidates.pop();
        segmentTree[nodeIndex].maxActiveValue = segmentTree[nodeIndex].activeRectangles.top();
        segmentTree[nodeIndex].pendingValue = segmentTree[nodeIndex].cleanupCandidates.top();
    }

    // Cleans up rectangles that should be flagged
    void cleanupNode(int nodeIndex, int currentMaxValue) {
        while (segmentTree[nodeIndex].pendingValue &&
               segmentTree[nodeIndex].pendingValue >= max(currentMaxValue, segmentTree[nodeIndex].minActiveValue)) {
            isFlagged[segmentTree[nodeIndex].pendingValue] = true;
            while (!segmentTree[nodeIndex].cleanupCandidates.empty() &&
                   (isRemoved[segmentTree[nodeIndex].cleanupCandidates.top()] ||
                    isFlagged[segmentTree[nodeIndex].cleanupCandidates.top()]))
                segmentTree[nodeIndex].cleanupCandidates.pop();
            segmentTree[nodeIndex].pendingValue = segmentTree[nodeIndex].cleanupCandidates.top();
        }
    }

    // Insert a rectangle in range [insertLeft, insertRight] into the segment tree
    void insertRectangleInRange(int nodeIndex, int leftIndex, int rightIndex,
                               int insertLeft, int insertRight,
                               int rectangleId, int currentMaxValue) {
        currentMaxValue = max(currentMaxValue, segmentTree[nodeIndex].maxActiveValue);
        if (leftIndex >= insertLeft && rightIndex <= insertRight) {
            addRectangleToNode(nodeIndex, rectangleId, currentMaxValue);
            updateSegmentTree(nodeIndex, leftIndex, rightIndex);
            return;
        }
        int midIndex = (leftIndex + rightIndex) / 2;
        if (insertLeft <= midIndex)
            insertRectangleInRange(nodeIndex * 2, leftIndex, midIndex, insertLeft, insertRight, rectangleId, currentMaxValue);
        if (insertRight > midIndex)
            insertRectangleInRange(nodeIndex * 2 + 1, midIndex + 1, rightIndex, insertLeft, insertRight, rectangleId, currentMaxValue);
        updateSegmentTree(nodeIndex, leftIndex, rightIndex);
    }

    // Remove a rectangle in range [removeLeft, removeRight] from the segment tree
    void removeRectangleInRange(int nodeIndex, int leftIndex, int rightIndex,
                               int removeLeft, int removeRight,
                               int rectangleId, int currentMaxValue) {
        currentMaxValue = max(currentMaxValue, segmentTree[nodeIndex].maxActiveValue);
        if (leftIndex >= removeLeft && rightIndex <= removeRight) {
            removeRectangleFromNode(nodeIndex, rectangleId, currentMaxValue);
            updateSegmentTree(nodeIndex, leftIndex, rightIndex);
            return;
        }
        int midIndex = (leftIndex + rightIndex) / 2;
        if (removeLeft <= midIndex)
            removeRectangleInRange(nodeIndex * 2, leftIndex, midIndex, removeLeft, removeRight, rectangleId, currentMaxValue);
        if (removeRight > midIndex)
            removeRectangleInRange(nodeIndex * 2 + 1, midIndex + 1, rightIndex, removeLeft, removeRight, rectangleId, currentMaxValue);
        updateSegmentTree(nodeIndex, leftIndex, rightIndex);
    }

    // Clean up the segment tree nodes globally after all insertions
    void globalCleanup(int nodeIndex, int leftIndex, int rightIndex, int currentMaxValue) {
        currentMaxValue = max(currentMaxValue, segmentTree[nodeIndex].maxActiveValue);
        if (!segmentTree[nodeIndex].cleanupThreshold || segmentTree[nodeIndex].cleanupThreshold < currentMaxValue) return;
        cleanupNode(nodeIndex, currentMaxValue);
        if (leftIndex == rightIndex) {
            updateSegmentTree(nodeIndex, leftIndex, rightIndex);
            return;
        }
        int midIndex = (leftIndex + rightIndex) / 2;
        globalCleanup(nodeIndex * 2, leftIndex, midIndex, currentMaxValue);
        globalCleanup(nodeIndex * 2 + 1, midIndex + 1, rightIndex, currentMaxValue);
        updateSegmentTree(nodeIndex, leftIndex, rightIndex);
    }

    int rectangleCount, nfCount;
public:
    // Read rectangles and prepare for coordinate compression
    void readRectangles() {
        scanf("%d%d", &rectangleCount, &nfCount);
        for (int rectIndex = 1; rectIndex <= rectangleCount; ++rectIndex) {
            int leftXInput, leftYInput, rightXInput, rightYInput;
            scanf("%d%d%d%d", &leftXInput, &leftYInput, &rightXInput, &rightYInput);
            rectangles[rectIndex] = { leftXInput + 1, leftYInput + 1, rightXInput, rightYInput };
            compressedX[++compressedX[0]] = leftXInput;
            compressedX[++compressedX[0]] = rightXInput;
            compressedY[++compressedY[0]] = leftYInput;
            compressedY[++compressedY[0]] = rightYInput;
        }
    }

    // Solve the rectangle coverage problem
    int solve() {
        // Coordinate compression
        sort(compressedX + 1, compressedX + compressedX[0] + 1);
        compressedX[0] = unique(compressedX + 1, compressedX + compressedX[0] + 1) - (compressedX + 1);
        sort(compressedY + 1, compressedY + compressedY[0] + 1);
        compressedY[0] = unique(compressedY + 1, compressedY + compressedY[0] + 1) - (compressedY + 1);

        // Prepare rectangle events (sweep line on x axis)
        for (int rectIndex = rectangleCount; rectIndex > 0; --rectIndex) {
            rectangles[rectIndex].leftX = lower_bound(compressedX + 1, compressedX + compressedX[0] + 1, rectangles[rectIndex].leftX) - compressedX;
            rectangles[rectIndex].rightX = lower_bound(compressedX + 1, compressedX + compressedX[0] + 1, rectangles[rectIndex].rightX) - compressedX;
            rectangles[rectIndex].leftY = lower_bound(compressedY + 1, compressedY + compressedY[0] + 1, rectangles[rectIndex].leftY) - compressedY;
            rectangles[rectIndex].rightY = lower_bound(compressedY + 1, compressedY + compressedY[0] + 1, rectangles[rectIndex].rightY) - compressedY;
            eventsAddAtX[rectangles[rectIndex].leftX].push_back({ rectangles[rectIndex].leftY, rectangles[rectIndex].rightY, rectIndex });
            eventsRemoveAtX[rectangles[rectIndex].rightX].push_back({ rectangles[rectIndex].leftY, rectangles[rectIndex].rightY, rectIndex });
        }

        // Build the segment tree on Y-axis
        buildSegmentTree(1, 1, compressedY[0]);

        int flaggedRectanglesCount = 0;
        // Sweep line along X
        for (int xIndex = 1; xIndex <= compressedX[0]; ++xIndex) {
            // Insert rectangles starting at this x
            for (const auto &event : eventsAddAtX[xIndex])
                insertRectangleInRange(1, 1, compressedY[0], event.fromY, event.toY, event.rectangleIndex, 0);
            // Cleanup after insertions
            globalCleanup(1, 1, compressedY[0], 0);
            // Remove rectangles ending at this x
            for (const auto &event : eventsRemoveAtX[xIndex]) {
                isRemoved[event.rectangleIndex] = true;
                removeRectangleInRange(1, 1, compressedY[0], event.fromY, event.toY, event.rectangleIndex, 0);
            }
        }
        // Count flagged rectangles
        for (int rectIndex = 1; rectIndex <= rectangleCount; ++rectIndex)
            if (isFlagged[rectIndex]) ++flaggedRectanglesCount;
        return flaggedRectanglesCount;
    }
public:
    int fCount()
    {
        return nfCount;
    }
};

int main() {
    RectangleCoverageSolver* solver = new RectangleCoverageSolver();
    solver->readRectangles();
    int flaggedCount = solver->solve();
    //printf("%d\n", flaggedCount);
    if(flaggedCount == solver->fCount())
        printf("Yes\n");
    else
        printf("No\n");
    delete solver;
    return 0;
}
