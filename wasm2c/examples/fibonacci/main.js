const { exec } = require('child_process');

const argValue = 5;  // Sample arguments
const command = `./fibonacci ${argValue}`;

exec(command, (error, stdout) => {
    if (error) {
        console.error(`Error: ${error.message}`);
        return;
    }
    console.log(`${stdout}`);
});