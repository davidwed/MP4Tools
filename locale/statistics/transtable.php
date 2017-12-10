<?php

$img_translated = "/translations/translated.png";
$img_fuzzy = "/translations/fuzzy.png";
$img_untranslated = "/translations/untranslated.png";
$graph_width = 200; 
$git_url = "https://sourceforge.net/p/mp4joiner/git/ci/master/tree/locale/";
$git_end ="?format=raw";
$potfile = "mp4joiner.pot";


$catstatus = array();
$authors = array();
$show_author = 0;

include '__catstatus.php';

?>

<table class="thintable">
<thead>
	<tr>
		<th>language</th>
		<th class="graph">graph</th>
		<th>translated</th>
		<th>fuzzy</th>
		<th>untrans.</th>
		<th>last updated</th>
	</tr>
<thead>
<tbody>
    <?php
        function get_langname($langcode)
        {
$langnames = array(
    "aa" => "Afar",
    "ab" => "Abkhazian",
    "ae" => "Avestan",
    "af" => "Afrikaans",
    "am" => "Amharic",
    "ar" => "Arabic",
    "as" => "Assamese",
    "ast" => "Asturian",
    "ay" => "Aymara",
    "az" => "Azerbaijani",
    "ba" => "Bashkir",
    "be" => "Belarusian",
    "bg" => "Bulgarian",
    "bh" => "Bihari",
    "bi" => "Bislama",
    "bn" => "Bengali",
    "bo" => "Tibetan",
    "br" => "Breton",
    "bs" => "Bosnian",
    "ca" => "Catalan",
    "ce" => "Chechen",
    "ch" => "Chamorro",
    "co" => "Corsican",
    "cs" => "Czech",
    "cu" => "Church Slavic",
    "cv" => "Chuvash",
    "cy" => "Welsh",
    "da" => "Danish",
    "de" => "German",
    "dz" => "Dzongkha",
    "el" => "Greek",
    "en" => "English",
    "eo" => "Esperanto",
    "es" => "Spanish",
    "et" => "Estonian",
    "eu" => "Basque",
    "fa" => "Persian",
    "fi" => "Finnish",
    "fj" => "Fijian",
    "fo" => "Faroese",
    "fr" => "French",
    "fur" => "Friulian",
    "fy" => "Frisian",
    "ga" => "Irish",
    "gd" => "Gaelic",
    "gl" => "Galician",
    "gn" => "Guarani",
    "gu" => "Gujarati",
    "ha" => "Hausa",
    "he" => "Hebrew",
    "hi" => "Hindi",
    "ho" => "Hiri Motu",
    "hr" => "Croatian",
    "hu" => "Hungarian",
    "hy" => "Armenian",
    "hz" => "Herero",
    "ia" => "Interlingua",
    "id" => "Indonesian",
    "ie" => "Interlingue",
    "ik" => "Inupiaq",
    "is" => "Icelandic",
    "it" => "Italian",
    "iu" => "Inuktitut",
    "ja" => "Japanese",
    "jw" => "Javanese",
    "ka" => "Georgian",
    "ki" => "Kikuyu",
    "kj" => "Kuanyama",
    "kk" => "Kazakh",
    "kl" => "Kalaallisut",
    "km" => "Khmer",
    "kn" => "Kannada",
    "ko" => "Korean",
    "ks" => "Kashmiri",
    "ku" => "Kurdish",
    "kv" => "Komi",
    "kw" => "Cornish",
    "ky" => "Kirghiz",
    "la" => "Latin",
    "lb" => "Letzeburgesch",
    "ln" => "Lingala",
    "lo" => "Lao",
    "lt" => "Lithuanian",
    "lv" => "Latvian",
    "mg" => "Malagasy",
    "mh" => "Marshall",
    "mi" => "Maori",
    "mk" => "Macedonian",
    "ml" => "Malayalam",
    "mn" => "Mongolian",
    "mo" => "Moldavian",
    "mr" => "Marathi",
    "ms" => "Malay",
    "mt" => "Maltese",
    "my" => "Burmese",
    "na" => "Nauru",
    "ne" => "Nepali",
    "ng" => "Ndonga",
    "nl" => "Dutch",
    "nn" => "Norwegian Nynorsk",
    "nb" => "Norwegian Bokmal",
    "nr" => "Ndebele, South",
    "nv" => "Navajo",
    "ny" => "Chichewa; Nyanja",
    "oc" => "Occitan",
    "om" => "(Afan) Oromo",
    "or" => "Oriya",
    "os" => "Ossetian; Ossetic",
    "pa" => "Panjabi",
    "pi" => "Pali",
    "pl" => "Polish",
    "ps" => "Pashto, Pushto",
    "pt" => "Portuguese",
    "qu" => "Quechua",
    "rm" => "Rhaeto-Romance",
    "rn" => "Rundi",
    "ro" => "Romanian",
    "ru" => "Russian",
    "rw" => "Kinyarwanda",
    "sc" => "Sardinian",
    "sd" => "Sindhi",
    "se" => "Northern Sami",
    "sg" => "Sangro",
    "sh" => "Serbo-Croatian",
    "si" => "Sinhalese",
    "sk" => "Slovak",
    "sl" => "Slovenian",
    "sm" => "Samoan",
    "sn" => "Shona",
    "so" => "Somali",
    "sq" => "Albanian",
    "sr" => "Serbian",
    "ss" => "Siswati",
    "st" => "Sesotho",
    "su" => "Sundanese",
    "sv" => "Swedish",
    "sw" => "Swahili",
    "ta" => "Tamil",
    "te" => "Telugu",
    "tg" => "Tajik",
    "th" => "Thai",
    "ti" => "Tigrinya",
    "tk" => "Turkmen",
    "tl" => "Tagalog",
    "tn" => "Setswana",
    "to" => "Tonga",
    "tr" => "Turkish",
    "ts" => "Tsonga",
    "tt" => "Tatarish",
    "tw" => "Twi",
    "ty" => "Tahitian",
    "ug" => "Uighur",
    "uk" => "Ukrainian",
    "ur" => "Urdu",
    "uz" => "Uzbek",
    "vi" => "Vietnamese",
    "vo" => "Volapuk",
    "wa" => "Walloon",
    "wo" => "Wolof",
    "xh" => "Xhosa",
    "yi" => "Yiddish",
    "yo" => "Yoruba",
    "za" => "Zhuang",
    "zh" => "Chinese",
    "zu" => "Zulu",
);
            if (strlen($langcode) > 2 && $langcode[2] == '_')
                $key=$langcode[0].$langcode[1];
            else
                $key=$langcode;
            return $langnames[$key];
        }

        function cmp_catstatus($a, $b)
        {
            $la = get_langname($a);
            $lb = get_langname($b);

            if ($la == $lb) {
                return 0;
            }
            return ($la < $lb) ? -1 : 1;
        }

        uksort($catstatus, "cmp_catstatus");
        
        while (list($lang, $value) = each ($catstatus))
        {
            $remark = "";
            $cnt_total = $value[0] + $value[1] + $value[2];
            if ($cnt_total == 0) {
                $cnt_total = 1;
                $remark = "<b> (BROKEN!)</b>";
            }
            $cnt_trans = 100 * $value[0] / $cnt_total;
            $cnt_fuzzy = 100 * $value[1] / $cnt_total;
            $cnt_untrans = 100 * $value[2] / $cnt_total;
            $w_trans = (int)($graph_width * $value[0] / $cnt_total);
            $w_fuzzy = (int)($graph_width * ($value[0] + $value[1]) / $cnt_total) - $w_trans;
            $w_untrans = (int)($graph_width * ($value[0] + $value[1] + $value[2]) / $cnt_total) - $w_trans - $w_fuzzy;
            if ($remark != "")
                echo "<tr class=\"error\">";
            else if ($value[1] + $value[2] > 0)
                echo "<tr class=\"incomplete\">";
            else
                echo "<tr class=\"complete\">";

            echo "<td class=\"lang\"><a href=\"$git_url$lang.po$git_end\">", get_langname($lang), " ($lang)</a>$remark</td>";
            
            echo "<td class=\"graph\">";
            if ($cnt_trans != 0) {
                echo "<img src=\"$img_translated\" width=\"$w_trans\" height=\"16\"/>";
            }
            if ($cnt_fuzzy != 0) {
                echo "<img src=\"$img_fuzzy\" width=\"$w_fuzzy\" height=\"16\"/>";
            }
            if ($cnt_untrans != 0) {
                echo "<img src=\"$img_untranslated\" width=\"$w_untrans\" height=\"16\"/>";
            }
            echo "</td>";
            printf("<td class=\"trans\">%0.1f %%</td>", $cnt_trans);
            printf("<td class=\"fuzzy\">%0.1f %%</td>", $cnt_fuzzy);
            printf("<td class=\"untrans\">%0.1f %%</td>", $cnt_untrans);
            echo "<td class=\"lastupdate\">$value[3]</td>";
            echo "</tr>\n";
        }
     ?>
</tbody>
</table>
