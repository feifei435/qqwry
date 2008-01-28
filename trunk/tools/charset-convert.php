<?php
$buffer = '';
$tmp_data_dir = './tmp_data';
$input_file = 'QQWry.Dat';
$output_file = 'QQWry.utf8.Dat';
define('RECORD_END',chr(0));
define('MODE_1',chr(1));
define('MODE_2',chr(2));

$input_fp=fopen($input_file,'rb');
$location_utf8_data='';
$total_larger=0;
$offset_larger = array();

if (!file_exists($tmp_data_dir.'/step1')) {
    $index=fread($input_fp,8);
    $index=unpack('V2',$index);
    $index_start=$index[1];
    $index_end=$index[2];
    echo 'index from ',$index_start,' to ',$index_end,"\n";
    fseek($input_fp,$index_start);
    while ($index_end!=ftell($input_fp)) {
        fseek($input_fp,4,SEEK_CUR);
        $data_indexes[]=current(unpack('V',fread($input_fp,3).chr(0)));
    }
    storeVar('index_start',$index_start);
    storeVar('index_end',$index_end);
    storeVar('data_indexes',$data_indexes);
    touch($tmp_data_dir.'/step1');
} else {
    echo "skip step 1\n";
    loadVar('index_start');
    loadVar('index_end');
    loadVar('data_indexes');
}

echo count($data_indexes)," indexes\n";

if (!file_exists($tmp_data_dir.'/step2')) {
    fseek($input_fp,8,SEEK_SET);
    $i=0;
    while ($index_start>ftell($input_fp)) {
        //echo "step 0 ... ";
        $c=fread($input_fp,1);
        $buffer=$c;
        if (in_array(ftell($input_fp)-1,$data_indexes)) {
            $buffer.=fread($input_fp,3);
        } else if ($c==MODE_1 || $c==MODE_2) {
            $buffer.=fread($input_fp,3);
        } else {
            while (ord($c=fread($input_fp,1))>0) {
                $buffer.=$c;
            }
            if ($c!=RECORD_END) {
                echo "fread error\n";
                exit(1);
            }
            $buffer_old_len=strlen($buffer);
            $buffer=iconv('GBK','UTF-8',$buffer);
            $total_larger += (strlen($buffer)-$buffer_old_len);
            $offset_larger[ftell($input_fp)]=$total_larger;
            //file_put_contents('tmp2.utf8',$buffer."\n",FILE_APPEND);
            $buffer.=$c;
        }
        $l=strlen($buffer);
        $i++;
        echo "\r",$i,'-------';
        $location_utf8_data.=$buffer;
    }
    file_put_contents($tmp_data_dir.'/location_utf8_data',$location_utf8_data);
    storeVar('total_larger',$total_larger);
    storeVar('offset_larger',$offset_larger);
    touch($tmp_data_dir.'/step2');
} else {
    echo "skip step 2\n";
    $location_utf8_data=file_get_contents($tmp_data_dir.'/location_utf8_data');
    loadVar('total_larger');
    loadVar('offset_larger');
}
echo "\ntotal larger:",$total_larger,"\n";

$output_fp=fopen($output_file,'ab');

if (!file_exists($tmp_data_dir.'/step3')) {
    echo 'patching header ... ';
    fwrite($output_fp,pack('V',$index_start+$total_larger));
    fwrite($output_fp,pack('V',$index_end+$total_larger));
    echo "ok\n";
    touch($tmp_data_dir.'/step3');
} else {
    echo "skip step 3\n";
}

if (!file_exists($tmp_data_dir.'/step4')) {
    echo 'patching data block ... ';
    $f_offset=0;
    for ($i=0;$i<strlen($location_utf8_data);$i++) {
        $c=$location_utf8_data[$i];
        if ($c==MODE_1 || $c==MODE_2) {
            $f_offset=current(unpack('V',
                    $location_utf8_data[$i+1]
                    .$location_utf8_data[$i+2]
                    .$location_utf8_data[$i+3]
                    .chr(0)
                    ));
            $f_offset_new=$f_offset;
            for ($n=0;$n<=$f_offset;$n++) {
                if (isset($offset_larger[$f_offset-$n])) {
                    $f_offset_new+=$offset_larger[$f_offset-$n];
                    break;
                }
            }
            if ($f_offset_new===$f_offset) {
                echo 'warning: new offset','===old offset(',$f_offset,') i:',$i,"\n";
            }
            $f_offset_new=pack('V',$f_offset_new);
            $location_utf8_data[$i+1] = $f_offset_new[0];
            $location_utf8_data[$i+2] = $f_offset_new[1];
            $location_utf8_data[$i+3] = $f_offset_new[2];
            $i+=3;
        }
    }
    fwrite($output_fp,$location_utf8_data);
    echo "ok\n";
    touch($tmp_data_dir.'/step4');
} else {
    echo "skip step 4\n";
}

unset($location_utf8_data);

if (!file_exists($tmp_data_dir.'/step5')) {
    echo 'patching index block (+',$total_larger,')... ';
    $i=0;
    fseek($input_fp,$index_start);
    $buffer='';
    while ($index_end!=ftell($input_fp)) {
        $buffer .= fread($input_fp,4);

        $f_offset=$data_indexes[$i];
        $f_offset_new=$f_offset;
        for ($n=0;$n<=$f_offset;$n++) {
            if (isset($offset_larger[$f_offset-$n])) {
                $f_offset_new+=$offset_larger[$f_offset-$n];
                break;
            }
        }
        if ($f_offset_new===$f_offset) {
            echo 'warning: new offset','===old offset(',$f_offset,') i:',$i,"\n";
        }

        $buffer .= substr(pack('V',$f_offset_new),0,3);
        fseek($input_fp,3,SEEK_CUR);
        $i++;
    }
    fwrite($output_fp,$buffer);
    echo "ok\n";
    touch($tmp_data_dir.'/step5');
} else {
    echo "skip step 5\n";
}

function storeVar($var,$val) {
    global $tmp_data_dir;
    file_put_contents($tmp_data_dir.'/'.$var,'<?php  $'.$var.'='.var_export($val,true).';');
}
function loadVar($var) {
    global $$var;
    global $tmp_data_dir;
    include $tmp_data_dir.'/'.$var;
}
