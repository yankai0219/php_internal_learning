<?php
    $var = "laruence";
    $d = $var;
        debug_zval_dump($d); //3
        debug_zval_dump($var); //3
            $var_dup = &$var;
            $mm = $var_dup;
                debug_zval_dump($d); //2
                debug_zval_dump($var); //1
                    debug_zval_dump($var_dup); //1
                    debug_zval_dump($var_dup); //1
                        debug_zval_dump($mm); //1

                        $var = 12000;
                            var_dump($var); //12000
                            var_dump($var_dup); //12000

                                var_dump($mm); //laruence
                                $mm = "12";
                                    var_dump($mm); //12
                                    var_dump($var_dup); //12000
                                        var_dump($var); //12000
