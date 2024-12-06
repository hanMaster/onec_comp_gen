use std::fs;
use std::io;
use std::path::Path;

use regex::Regex;

pub fn copy_directory(source: String, destination: String) -> io::Result<()> {
    // Create destination directory if it doesn't exist
    if !Path::new(&destination).exists() {
        fs::create_dir_all(&destination)?;
    }

    // Read source directory entries
    for entry in fs::read_dir(&source)? {
        let entry = entry?;
        let entry_path = entry.path();
        let file_name = entry.file_name();
        let destination_path = Path::new(&destination).join(file_name);

        if entry_path.is_dir() {
            // Recursively copy subdirectories
            copy_directory(
                entry_path.to_string_lossy().to_string(),
                destination_path.to_string_lossy().to_string()
            )?;
        } else {
            // Copy files
            fs::copy(&entry_path, &destination_path)?;
        }
    }

    Ok(())
}


pub fn replace_text_in_file(file_path: &String, target_text: String, new_text: String) -> io::Result<()> {
    let content = fs::read_to_string(&file_path)?;
    let new_content = content.replace(&target_text, &new_text);
    fs::write(&file_path, new_content)?;
    
    Ok(())
}

pub fn replace_text_in_file_regex(file_path: &String, pattern: &str, new_text: &str) -> io::Result<()> {
    let content = fs::read_to_string(file_path)?;
    let regex = Regex::new(pattern)
        .map_err(|e| io::Error::new(io::ErrorKind::InvalidInput, e))?;
    let new_content = regex.replace_all(&content, new_text);
    fs::write(file_path, new_content.as_bytes())?;
    
    Ok(())
}

pub fn exists_base_template(file_path: String) -> bool {
    Path::new(&file_path).exists()
}