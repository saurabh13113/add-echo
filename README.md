# add-echo 🔊
*University project that removes vocals and adds an echoing sound to a audio(wav) file by decoding bit information.*

For this project project, I developed two C programs aimed at manipulating audio files: one to remove vocals from a stereo audio file and another to add an echo effect to a mono audio file. This assignment was a compelling blend of audio processing, command-line interfaces, and dynamic memory management, allowing me to apply and expand my understanding of C programming.

## remvocals.c: Removing Vocals from Stereo Audio Files
The first program, remvocals.c, was designed to remove vocals from stereo PCM WAV files. The process involved the following key steps:

*Reading and Preserving the Header:* The WAV file header contains critical information about the audio file. Thus, the first 44 bytes of the input file were copied directly to the output file without modification. This ensured that the output file maintained the necessary format and metadata.\
*Processing Audio Data:* The core algorithm to remove vocals was based on the principle of channel subtraction. By subtracting the right channel's audio data from the left channel's data for each sample and averaging the result, we effectively removed the vocals, which were common to both channels. The formula used was: **combined = (left − right)/2**\
This approach leverages the fact that vocals are often recorded identically in both channels, whereas other sounds vary between channels.\
*Writing Processed Data:* The resulting combined audio data was then written to the output file, replacing the original stereo channels.

This program honed my skills in file I/O, error handling, and understanding of audio file formats. The use of fread and fwrite functions ensured efficient and reliable file operations.

## addecho.c: Adding Echo to Mono Audio Files
The second program, addecho.c, added an echo effect to mono PCM WAV files. This task was more complex and required careful manipulation of audio data and dynamic memory management:

*Command-Line Processing:* Utilizing the getopt function, the program accepted optional parameters for delay and volume scale, with defaults set to 8000 and 4, respectively. This flexibility allowed for customizable echo effects.\
*Header Modification:* Unlike remvocals.c, this program had to adjust the WAV file header to reflect the new file size, accounting for the added echo. Specifically, the size values at byte positions 4 and 40 were updated.\
*Echo Buffer Management:* An echo buffer was dynamically allocated based on the delay parameter. This buffer stored the delayed samples scaled by the volume scale factor. The audio processing followed these phases:\
*Initial Phase:* Directly copied samples from the original file to the output file until reaching the delay point.\
*Mixing Phase:* Started mixing the original samples with the scaled samples from the echo buffer.\
*Post-Processing Phase:* Continued writing samples from the echo buffer after the original samples had been processed, ensuring the echo effect was fully captured.\
*Memory Management:* Dynamic allocation using malloc was essential for handling the echo buffer. This experience significantly enhanced my understanding of dynamic memory and the importance of efficient memory usage in audio processing.

**Design Choices and Learnings**
Throughout the project, several design choices were made to ensure robustness and efficiency:

*Binary Mode for File Operations:* Opening files in binary mode preserved the integrity of the audio data, crucial for accurate processing.\
*Error Checking:* Comprehensive error handling was implemented to manage file operations and memory allocation, preventing crashes and ensuring graceful exits on failure.\
*Optimized Processing:* The use of fread and fwrite allowed for efficient block processing of audio data, reducing the overhead associated with single-sample operations.

This project not only solidified my knowledge of C programming but also provided practical insights into audio processing techniques and the intricacies of working with audio file formats. The hands-on experience with command-line tools and dynamic memory has been invaluable, equipping me with skills that are directly applicable to real-world software development scenarios.
