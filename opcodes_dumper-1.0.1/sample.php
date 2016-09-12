<?php
function test($c)
{
    static $st = 0;
    $st++;
    return $st + $c;
}

$c = 12;
$d = test($c);
$e = test($c);
echo $d . "\n";
echo $e . "\n";
