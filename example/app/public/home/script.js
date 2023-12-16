// Fetch Feature Items
const featureItems = document.querySelectorAll(".features__list > button");
const featureContents = document.querySelectorAll(".features__content > div");

const handleFeatureClick = (event) => {
  let buttonId = "";

  if (event.target.tagName.toLowerCase() === "li") {
    buttonId = event.target.parentElement.id;
  } else {
    buttonId = event.target.id;
  }

  // reset all
  featureItems.forEach((item) => item.classList.remove("active"));
  featureContents.forEach((item) => item.classList.remove("active"));

  // Add active to the target and the content div
  const content = document.getElementById(`${buttonId}-content`);
  const button = document.getElementById(buttonId);

  button.classList.add("active");
  content.classList.add("active");
};

// Features Page
featureItems.forEach((item) =>
  item.addEventListener("click", handleFeatureClick)
);
