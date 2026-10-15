**Short version: score every unused column by how many clashing pairs it splits, take the best, split the groups by that column's letter, repeat.**

**Setup**

1. Put keywords of the same length in the same group. Different lengths are already apart.
2. Ignore any group with only 1 keyword. It has 0 pairs, so it's done.

**One round**

For each unused column, go through every group that has 2 or more keywords:

1. **Group pairs:** n × (n − 1) ÷ 2.
2. **Same pairs, using a counter per letter:** reset all counters to 0. Walk the keywords in the group. For each one, take its letter in this column, add that letter's counter to `same`, then add 1 to the counter.
3. **Group separated** = group pairs − same.
4. If the group's keywords are too short to have this column, its separated is 0.
5. Add up the separated of all groups. That is the column's score.

Then:

1. Take the column with the highest score. Ties: either.
2. If the highest score is 0 and groups with 2 or more keywords remain, those keywords are identical. Stop.
3. Split every group by the letter in the chosen column. Keywords stay together only if they have the same letter there.
4. Drop groups of size 1.
5. If no group is left, you're done. Otherwise start a new round using only the groups that remain.

**Trace on aabb, aaab, aacc, aaca, abac, acac**

- **Round 1:** one group of 6, so 15 pairs. Col 0 separates 0, col 1 separates 9, col 2 separates 11, col 3 separates 11. Take col 2 (tie with col 3).
- **Split by col 2:** aabb is alone and done. Group A is aaab, abac, acac (3 pairs). Group B is aacc, aaca (1 pair).
- **Round 2:** 4 pairs left. Col 0 separates 0. Col 1 separates 3 (all in group A). Col 3 separates 3 (2 in group A, 1 in group B). Take col 1.
- **Split by col 1:** aaab, abac and acac all separate. Only aacc and aaca remain.
- **Round 3:** col 3 separates the last pair (c vs a). Done.

Columns chosen: 2, 1, 3. Col 0 was never needed.

**Notes for bump**

**No fixed column order is needed. On each clash, bump the one score (a letter in a column) that disturbs the fewest placed keywords.** Going column 0, then 1, then 2 also works, but it can move more keywords than necessary.

With per-column tables, every (column, letter) pair is its own dial. Turning a dial shifts every keyword that has that letter in that column, like sliding a whole row of seats. You want the dial that moves the fewest people.

When the new keyword K lands on the slot of keyword P:

1. **List the dials.** Look at each chosen column where K and P have different letters. Each such column gives two dials: (column, K's letter) and (column, P's letter). Where their letters match, a bump moves both equally and changes nothing between them.
2. **Cost of a dial** = how many placed keywords, plus K, have that letter in that column. It's the same letter count you already take in Pass 1.
3. **Take the cheapest dial.** For ties, take the earliest column so the result is predictable.
4. **Raise it by 1.** Recompute the hash of every keyword on that dial. Each one should land on a free slot, and the slot it just left counts as free.
5. **If any lands on a taken slot,** raise the same dial again and repeat.
6. **If the dial hits your max score,** put it back and try the next cheapest dial.

A per-column table makes the cost smaller than a shared table does. A bump on a shared table moves the letter in every column at once, while here it moves only one column.
