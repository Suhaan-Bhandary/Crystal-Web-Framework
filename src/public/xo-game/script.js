const PLAYER_ONE = 1;
const PLAYER_ONE_MARK = "X";

const PLAYER_TWO_AI = -1;
const PLAYER_TWO_AI_MARK = "O";

const DRAW = "D";

let player_playing = PLAYER_ONE;
let isGameEnded = false;
let isAIPlaying = false;

const grid = [
  [" ", " ", " "],
  [" ", " ", " "],
  [" ", " ", " "],
];

const handleWrongMove = () => {
  alert("Please Play a valid move");
};

const checkPlayerWon = (grid) => {
  // check rows
  for (let row = 0; row < 3; row++) {
    if (
      grid[row][0] != " " &&
      grid[row][0] == grid[row][1] &&
      grid[row][0] == grid[row][2]
    ) {
      return grid[row][0];
    }
  }

  // check cols
  for (let col = 0; col < 3; col++) {
    if (
      grid[0][col] != " " &&
      grid[0][col] == grid[1][col] &&
      grid[0][col] == grid[2][col]
    ) {
      return grid[0][col];
    }
  }

  // check diagonal
  if (
    grid[0][0] != " " &&
    grid[0][0] == grid[1][1] &&
    grid[0][0] == grid[2][2]
  ) {
    return grid[0][0];
  }

  // check secondary diagonal
  if (
    grid[0][2] != " " &&
    grid[0][2] == grid[1][1] &&
    grid[0][2] == grid[2][0]
  ) {
    return grid[0][2];
  }

  // Check for draw
  let count = 0;
  for (let row = 0; row < 3; row++) {
    for (let col = 0; col < 3; col++) {
      if (grid[row][col] == " ") count++;
    }
  }

  if (count == 0) return DRAW;
  return null;
};

const highlightWin = (color) => {
  // check rows
  for (let row = 0; row < 3; row++) {
    if (
      grid[row][0] != " " &&
      grid[row][0] == grid[row][1] &&
      grid[row][0] == grid[row][2]
    ) {
      document.getElementById(`cell-${row}-${0}`).style.color = color;
      document.getElementById(`cell-${row}-${1}`).style.color = color;
      document.getElementById(`cell-${row}-${2}`).style.color = color;
      return;
    }
  }

  // check cols
  for (let col = 0; col < 3; col++) {
    if (
      grid[0][col] != " " &&
      grid[0][col] == grid[1][col] &&
      grid[0][col] == grid[2][col]
    ) {
      document.getElementById(`cell-${0}-${col}`).style.color = color;
      document.getElementById(`cell-${1}-${col}`).style.color = color;
      document.getElementById(`cell-${2}-${col}`).style.color = color;
      return;
    }
  }

  // check diagonal
  if (
    grid[0][0] != " " &&
    grid[0][0] == grid[1][1] &&
    grid[0][0] == grid[2][2]
  ) {
    document.getElementById(`cell-${0}-${0}`).style.color = color;
    document.getElementById(`cell-${1}-${1}`).style.color = color;
    document.getElementById(`cell-${2}-${2}`).style.color = color;
    return;
  }

  // check secondary diagonal
  if (
    grid[0][2] != " " &&
    grid[0][2] == grid[1][1] &&
    grid[0][2] == grid[2][0]
  ) {
    document.getElementById(`cell-${0}-${2}`).style.color = color;
    document.getElementById(`cell-${1}-${1}`).style.color = color;
    document.getElementById(`cell-${2}-${0}`).style.color = color;
    return;
  }
};

const handleGameOver = (player_won) => {
  isGameEnded = true;
  document.getElementById("gameOverContainer").classList.add("active");

  const gameOverText = document.getElementById("gameOverText");
  if (player_won == DRAW) {
    gameOverText.textContent = "Draw";
  } else if (player_won == PLAYER_ONE_MARK) {
    highlightWin("green");
    gameOverText.textContent = "Player won";
  } else {
    highlightWin("red");
    gameOverText.textContent = "AI won";
  }
};

const handlePlay = (row, col) => {
  // Play
  if (player_playing == PLAYER_ONE) grid[row][col] = PLAYER_ONE_MARK;
  else grid[row][col] = PLAYER_TWO_AI_MARK;

  // Rerender the grid
  generateGrid();

  // Check if player won or not
  const player_won = checkPlayerWon(grid);
  if (player_won) handleGameOver(player_won);

  // make the other play play
  player_playing = -player_playing;
};

const minMax = (player_playing, grid) => {
  const player_won = checkPlayerWon(grid);
  if (player_won == DRAW) return 0;
  if (player_won == PLAYER_TWO_AI_MARK) return 1;
  if (player_won == PLAYER_ONE_MARK) return -1;

  let bestMoveValue = -1000;
  if (player_playing == PLAYER_ONE) bestMoveValue = 1000;

  for (let i = 0; i < 3; i++) {
    for (let j = 0; j < 3; j++) {
      if (grid[i][j] == " ") {
        // we use sign as -1 as now other player will play
        if (player_playing == PLAYER_ONE) grid[i][j] = PLAYER_ONE_MARK;
        else grid[i][j] = PLAYER_TWO_AI_MARK;

        const val = minMax(-player_playing, grid);
        grid[i][j] = " ";

        if (player_playing == PLAYER_TWO_AI) {
          if (val > bestMoveValue) bestMoveValue = val;
        } else {
          if (val < bestMoveValue) bestMoveValue = val;
        }
      }
    }
  }

  return bestMoveValue;
};

const getAIMove = (grid) => {
  let bestMoveValue = -1000;
  let bestMove = [-1, -1];

  for (let i = 0; i < 3; i++) {
    for (let j = 0; j < 3; j++) {
      if (grid[i][j] == " ") {
        // we use sign as -1 as now other player will play
        grid[i][j] = PLAYER_TWO_AI_MARK;
        const val = minMax(PLAYER_ONE, grid);
        grid[i][j] = " ";

        if (val > bestMoveValue) {
          bestMoveValue = val;
          bestMove = [i, j];
        }
      }
    }
  }

  return bestMove;
};

const AIPlay = () => {
  isAIPlaying = true;

  const dummyGrid = structuredClone(grid);
  const [row, col] = getAIMove(dummyGrid);

  handlePlay(row, col);

  isAIPlaying = false;
};

const handleCellClick = (row, col) => {
  if (isGameEnded) return;
  if (isAIPlaying) return;

  // Check if move is valid or not
  if (grid[row][col] != " ") {
    handleWrongMove(row, col);
    return;
  }

  handlePlay(row, col);
  if (!isGameEnded) AIPlay();
};

const generateGrid = () => {
  const gridElement = document.getElementById("grid");
  gridElement.innerHTML = "";

  const tbody = document.createElement("tbody");

  for (let i = 0; i < 3; i++) {
    const row = document.createElement("tr");
    for (let j = 0; j < 3; j++) {
      const cell = document.createElement("td");
      cell.textContent = grid[i][j];
      cell.id = `cell-${i}-${j}`;
      cell.addEventListener("click", () => handleCellClick(i, j));

      row.appendChild(cell);
    }

    tbody.appendChild(row);
  }

  gridElement.appendChild(tbody);
};

const restartGame = () => {
  player_playing = PLAYER_ONE;
  isGameEnded = false;
  document.getElementById("gameOverContainer").classList.remove("active");

  for (let i = 0; i < 3; i++) {
    for (let j = 0; j < 3; j++) {
      grid[i][j] = " ";
    }
  }

  generateGrid();
};

// Main code
document.getElementById("restart").addEventListener("click", restartGame);
generateGrid();
