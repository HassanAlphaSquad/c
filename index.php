<title>PHP page</title>
<?php
echo "<h1 style='text-align:center'>Hello World!</h1>";
$colour = "red";
switch ($colour) {
    case "red":
        echo "Roses are red!";
        break;
    case "blue":
        echo "Violets are blue!";
        break;
    default:
        echo "Life is colourless!";
        break;
}

echo "<br/>";

$status = [
    0 => "Not started",
    1 => "In progress",
    2 => "Completed",
    3 => "Approved"
];

foreach ($status as $key => $value) {
    echo "<strong>$key</strong> => $value <br/>";
}
