The brute-force method to test line intersect require O(n^2) time. 

But the intersection points often less than n^2, we want an algorithm whose running time depends not only on the number of segments, but also on the number of intersection points. Such an algorithm is called an *output-sensitive algorithm*: the running time of the algorithm is sensitive to the size of the output. 

A simple idea is that the pair of segments which is far apart can't intersect. We can sort the segments by the orthogonal projection on the y-axis, and then use a horizontal line called *sweep line* to sweep the plane. This type of algorithm is called a *plane sweep algorithm*.