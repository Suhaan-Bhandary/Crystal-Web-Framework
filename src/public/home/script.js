const popUpButton = document.getElementById('popUpButton');
const closeButton = document.getElementById('closeButton');
const popUpWindow = document.getElementById('popUpWindow');

popUpButton.addEventListener('click', () => {
  popUpWindow.classList.toggle('active');
});

closeButton.addEventListener('click', () => {
  popUpWindow.classList.toggle('active');
});

popUpWindow.addEventListener('click', (event) => {
  if (event.target.id !== 'popUpWindow') return;
  popUpWindow.classList.toggle('active');
});
