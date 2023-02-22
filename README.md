# unpbasic
Unprotects an encrypted GWBASIC program, detokenizes and outputs it to the console.

<pre>
Usage: unpbasic &lt;file&gt;
</pre>

# Notes
* Currently only outputs to the console. Redirect it if you want to save it.
* Floating point output isn't 100% faithful to GWBASIC output. Always check your output.
* I may later add a few command line switches to only-unprotect or only-detokenize. If you want to add these using getopt, feel free to submit it.

# License Notes
Portions of this code are adapted from Ryan Juckett's (http://www.ryanjuckett.com/) implementation of the Dragon 4 algorithm. His code remains his, and the files which contain adaptations of his code has his notice, as a way of crediting him. I retain rights to my own code, licensing it GPL3. If you use my code in your own, please credit me and share this repository. If his code is incompatible with the GPL3, please feel free to reach out to me regarding this. But based on his notes on his website, I think he would probably be fine with this arrangement.
