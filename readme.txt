My first AI chose the first allowed move it found.
After trying a minmax algorithm, I decided heuristics worked better and
decided my tournament AI would rely on heuristics alone.
I decided corners needed to be weighed most heavily, so I gave corners
a high score. I weighed piece flips about 20% lower than corners.

I also added a mobility component to the model based on how many possible moves each side has after a move, giving it a small weight.
I expect having more moves to be beneficial and giving an opponent few choices to also be beneficial.

Since pieces flip so quickly, I think that minmax algorithms are not very effective, even to three levels, and a good heuristic will outperform it.
