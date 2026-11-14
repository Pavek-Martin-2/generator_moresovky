
<# gkostoulias(at)gmail.com
'ps.morsecode.ps1' Morse Code Encoder/Decoder #>

<#
Input Morse code or plain text using "." for a dit and "-" or "_" for a dah.
Letters in Morse code should be separated by spaces and words by "/" or "|".
#>

 function MorseCode
	{
	[cmdletbinding()]
	param (
		[parameter(mandatory=$true,
		valuefrompipeline=$true)]
		[string]$input
	)

	$indexArray = @(
		'a','b','c','d','e','f','g','h','i','j','k',
		'l','m','n','o','p','q','r','s','t','u','v',
		'w','x','y','z','0','1','2','3','4','5','6',
		'7','8','9','.','?','!',':',';','-','_','/',
        "'",'"','@','&','=','+','$'
	) # na konci schvalene jeden prazdjej pro chyby
      # v pripade ze nejnoi znak tak prehrava posledni...

      # neumi to  \ | [] {} % ^ * ` ~
        # co nejde pridat do pole najednou tak je tady pridano vzlast FUNGUJE
        $indexArray += "," # ascii 44
        $indexArray += "(" # ascii 40
        $indexArray += ")" # ascii 41
        $indexArray += "#" # ascii 35
        $indexArray += ""
        #echo $indexArray.Length

    	$morseArray = @(
		'.-','-...','-.-.','-..','.','..-.','--.',
		'....','..','.---','-.-','.-..','--','-.',
		'---','.--.','--.-','.-.','...','-','..-',
		'...-','.--','-..-','-.--','--..','-----',
		'.----','..---','...--','....-','.....',
		'-....','--...','---..','----.',
        '.-.-.-','..--..','-.-.--',
        '---...','-.-.-.','-....-','..--.-',
        '-..-.','.----.','.-..-.','.--.-.',
        '.-...','-...-','.-.-.','...-..-',
        '--..--','-.--.','-.--.-','---...',''
	) # tady na konci taky jeden schvalne volnej, pro chyby

      # $morseArray.Length
    
		($input.ToCharArray() | %{ ($_ + ':') }).ToCharArray() |
		% {
			if ($_ -eq ':')
				{
				#letter timing
				write-host ' ' -n
				sleep -m 300
				return
			}
			if ($_ -eq ' ')
				{
				#word timing = (letter + word timing)
				write-host -ForegroundColor white '|' -n # -n = -NoNewline (totez)
				sleep -m 600
				return
			}

			$i = $indexArray.indexof("$_")
			$morseArray[$i] -split "(.)" |
			% {
				if ($_ -eq '.') { dit }
				if ($_ -eq '-') { dah }
                   
			}
            write-host -ForegroundColor red $indexArray[$i].toUpper() -NoNewline # TOTO JE PRIDANO JAKO MOJE
		}
		write ''
	}
	

 function dit
	{
	write-host -ForegroundColor cyan "." -n
	[console]::beep(600,100) # 600,100
 }

 function dah
	{
	write-host -ForegroundColor cyan "-" -n
	[console]::beep(600,300) # 600,300
 }


cls
Write-Host -ForegroundColor cyan "! - musi bejt neco predtim napr. ahoj !"
Write-Host -ForegroundColor cyan 'znak "#" je nahrada za znak ":" '
Write-Host -ForegroundColor cyan 'neumi to  \ | [] {} % ^ * "`" ~'
Write-Host -ForegroundColor red "text = quit (pro ukonceni)"

for (;;){ # infinite loop
[string] $text = Read-Host "text "

if ( $text -notlike "quit" ) {


$text = $text.ToLower()
echo $text

$text | MorseCode

}else{
Write-Host -ForegroundColor red "konec"
exit
}
}

