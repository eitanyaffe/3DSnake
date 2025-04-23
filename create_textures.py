import numpy as np
from PIL import Image, ImageDraw, ImageFont

def create_brick_texture(size=256):
    # Create a bubble gum pink brick pattern
    img = Image.new('RGB', (size, size), color='white')
    draw = ImageDraw.Draw(img)
    
    # Define colors with more variation
    base_colors = [
        (255, 182, 193),   # Light pink
        (255, 192, 203),   # Pink
        (255, 174, 185),   # Pale pink
        (255, 160, 180),   # Deep pink
        (255, 150, 170),   # Medium pink
        (255, 140, 160),   # Dark pink
    ]
    mortar_color = (255, 220, 225)  # Light pink mortar
    
    # Create noise pattern for surface roughness
    noise = np.random.rand(size, size) * 20 - 10
    
    # Draw base mortar lines with slight variation
    for y in range(0, size, 32):
        # Add slight variation to mortar line
        offset = np.random.randint(-2, 3)
        draw.line([(0, y + offset), (size, y + offset)], fill=mortar_color, width=3)
    
    # Create a more natural brick pattern
    row_height = 32
    standard_width = 64
    y = 0
    
    while y < size:
        # Alternate between full and half-brick offset for each row
        x_offset = 0 if (y // row_height) % 2 == 0 else standard_width // 2
        x = -x_offset  # Start slightly off-screen to allow for edge bricks
        
        while x < size:
            # Vary brick width slightly
            width_variation = np.random.randint(-8, 9)
            brick_width = standard_width + width_variation
            
            # Vary brick height slightly
            height_variation = np.random.randint(-4, 5)
            brick_height = row_height + height_variation
            
            # Choose random base color for this brick
            base_color = base_colors[np.random.randint(0, len(base_colors))]
            
            # Add slight color variation
            color_variation = np.random.randint(-10, 11)
            brick_color = tuple(max(0, min(255, c + color_variation)) for c in base_color)
            
            # Create gradient effect for each brick
            for dy in range(brick_height):
                for dx in range(brick_width):
                    # Calculate position within brick (0 to 1)
                    px = dx / brick_width
                    py = dy / brick_height
                    
                    # Add noise to position
                    noise_x = int(x + dx) % size
                    noise_y = int(y + dy) % size
                    noise_value = noise[noise_y, noise_x]
                    
                    # Calculate lighting factors
                    edge_factor = min(px, 1-px, py, 1-py) * 2  # Edge lighting
                    top_light = (1 - py) * 0.3  # Top lighting
                    right_shadow = px * 0.2  # Right shadow
                    
                    # Combine all factors
                    light_factor = 1 + top_light - right_shadow + edge_factor + noise_value/255
                    
                    # Apply lighting to color
                    pixel_color = tuple(max(0, min(255, int(c * light_factor))) for c in brick_color)
                    
                    # Draw pixel
                    draw.point((x + dx, y + dy), fill=pixel_color)
            
            # Add surface variations (spots and marks)
            for _ in range(4):
                var_x = x + np.random.randint(2, brick_width-2)
                var_y = y + np.random.randint(2, brick_height-2)
                var_size = np.random.randint(2, 5)
                
                # Create more natural-looking spots
                for dx in range(-var_size, var_size+1):
                    for dy in range(-var_size, var_size+1):
                        if dx*dx + dy*dy <= var_size*var_size:  # Circular spot
                            spot_x = var_x + dx
                            spot_y = var_y + dy
                            if 0 <= spot_x < size and 0 <= spot_y < size:
                                # Vary the spot color
                                spot_color = tuple(max(0, min(255, c + np.random.randint(-30, 31))) for c in brick_color)
                                draw.point((spot_x, spot_y), fill=spot_color)
            
            # Add occasional small cracks with varying width
            if np.random.random() < 0.3:
                crack_x = x + np.random.randint(5, brick_width-5)
                crack_y = y + np.random.randint(5, brick_height-5)
                crack_length = np.random.randint(3, 8)
                crack_angle = np.random.randint(0, 360)
                end_x = crack_x + int(crack_length * np.cos(np.radians(crack_angle)))
                end_y = crack_y + int(crack_length * np.sin(np.radians(crack_angle)))
                
                # Draw crack with varying width
                for i in range(3):
                    offset = i - 1
                    draw.line([(crack_x + offset, crack_y), (end_x + offset, end_y)], 
                             fill=tuple(max(0, c - 60) for c in brick_color), width=1)
            
            x += brick_width
        
        y += row_height
    
    return img

def create_wall_texture(size=256):
    # Create a sophisticated wall texture
    img = Image.new('RGB', (size, size), color='white')
    draw = ImageDraw.Draw(img)
    
    # Define base colors for the wall
    base_colors = [
        (180, 180, 180),  # Light gray
        (160, 160, 160),  # Medium gray
        (140, 140, 140),  # Dark gray
        (170, 170, 170),  # Warm gray
        (150, 150, 150),  # Cool gray
    ]
    
    # Create noise patterns for different effects
    roughness_noise = np.random.rand(size, size) * 30 - 15  # Surface roughness
    metallic_noise = np.random.rand(size, size) * 40 - 20   # Metallic variation
    highlight_noise = np.random.rand(size, size) * 25 - 12  # Highlight variation
    
    # Create stone pattern with varying sizes
    stone_size = 32
    for y in range(0, size, stone_size):
        for x in range(0, size, stone_size):
            # Vary stone size slightly
            size_variation = np.random.randint(-4, 5)
            current_size = stone_size + size_variation
            
            # Choose base color with slight variation
            base_color = base_colors[np.random.randint(0, len(base_colors))]
            color_variation = np.random.randint(-10, 11)
            stone_color = tuple(max(0, min(255, c + color_variation)) for c in base_color)
            
            # Draw each stone with per-pixel effects
            for dy in range(current_size):
                for dx in range(current_size):
                    if x + dx < size and y + dy < size:
                        # Calculate position within stone (0 to 1)
                        px = dx / current_size
                        py = dy / current_size
                        
                        # Get noise values
                        noise_x = (x + dx) % size
                        noise_y = (y + dy) % size
                        roughness = roughness_noise[noise_y, noise_x]
                        metallic = metallic_noise[noise_y, noise_x]
                        highlight = highlight_noise[noise_y, noise_x]
                        
                        # Calculate lighting factors
                        edge_factor = min(px, 1-px, py, 1-py) * 2  # Edge lighting
                        top_light = (1 - py) * 0.4  # Top lighting
                        right_shadow = px * 0.3  # Right shadow
                        
                        # Metallic effect
                        metallic_factor = (1 + np.sin(px * 8 + py * 8) * 0.2) * (1 + metallic/255)
                        
                        # Combine all factors
                        light_factor = (1 + top_light - right_shadow + edge_factor + 
                                      roughness/255 + highlight/255) * metallic_factor
                        
                        # Apply lighting to color
                        pixel_color = tuple(max(0, min(255, int(c * light_factor))) for c in stone_color)
                        
                        # Draw pixel
                        draw.point((x + dx, y + dy), fill=pixel_color)
            
            # Add subtle cracks
            if np.random.random() < 0.2:
                crack_x = x + np.random.randint(5, current_size-5)
                crack_y = y + np.random.randint(5, current_size-5)
                crack_length = np.random.randint(3, 8)
                crack_angle = np.random.randint(0, 360)
                end_x = crack_x + int(crack_length * np.cos(np.radians(crack_angle)))
                end_y = crack_y + int(crack_length * np.sin(np.radians(crack_angle)))
                
                # Draw crack with varying width
                for i in range(2):
                    offset = i - 0.5
                    draw.line([(crack_x + offset, crack_y), (end_x + offset, end_y)], 
                             fill=tuple(max(0, c - 40) for c in stone_color), width=1)
            
            # Add occasional reflective spots
            if np.random.random() < 0.1:
                spot_x = x + np.random.randint(2, current_size-2)
                spot_y = y + np.random.randint(2, current_size-2)
                spot_size = np.random.randint(2, 4)
                
                # Create reflective spot
                for dx in range(-spot_size, spot_size+1):
                    for dy in range(-spot_size, spot_size+1):
                        if dx*dx + dy*dy <= spot_size*spot_size:
                            spot_pos_x = spot_x + dx
                            spot_pos_y = spot_y + dy
                            if 0 <= spot_pos_x < size and 0 <= spot_pos_y < size:
                                # Make spot slightly brighter
                                spot_color = tuple(min(255, c + 30) for c in stone_color)
                                draw.point((spot_pos_x, spot_pos_y), fill=spot_color)
    
    # Add subtle mortar lines
    mortar_color = (100, 100, 100)  # Dark gray mortar
    for y in range(0, size, stone_size):
        offset = np.random.randint(-2, 3)
        draw.line([(0, y + offset), (size, y + offset)], fill=mortar_color, width=1)
    
    for x in range(0, size, stone_size):
        offset = np.random.randint(-2, 3)
        draw.line([(x + offset, 0), (x + offset, size)], fill=mortar_color, width=1)
    
    return img

def create_help_texture(size=512):  # Increased resolution to 512x512
    # Create a help window texture with bright white background
    img = Image.new('RGB', (size, size), color=(240, 240, 240))  # Bright white background
    draw = ImageDraw.Draw(img)
    
    # Define control instructions with lowercase keys
    controls = [
        "n: New game",
        "a/s/x/z: Turns",
        "c/n: Rolls",
        "space: Pause",
        "e: Eye mode",
        "esc: Exit"
    ]
    
    # Load a font
    try:
        font = ImageFont.truetype("/System/Library/Fonts/Helvetica.ttc", 48)
    except:
        font = ImageFont.load_default()
    
    # Calculate total height needed
    line_spacing = 80
    total_height = len(controls) * line_spacing
    
    # Calculate starting y position to center vertically
    start_y = (size - total_height) // 2
    
    # Left margin for text
    left_margin = 50
    
    # Draw control instructions
    y = start_y
    for text in controls:
        # Draw the text at left margin
        draw.text((left_margin, y), text, fill=(0, 0, 0), font=font)  # Black text
        y += line_spacing
    
    # Flip the image vertically to match OpenGL's texture coordinates
    img = img.transpose(Image.FLIP_TOP_BOTTOM)
    
    return img

def create_snake_texture(size=256):
    # Create an orange snake texture
    img = Image.new('RGB', (size, size), color='white')
    draw = ImageDraw.Draw(img)
    
    # Create base pattern with orange color
    for y in range(size):
        for x in range(size):
            # Create a scaly pattern with varying shades of orange
            scale = int(np.sin(x/15) * 8 + np.cos(y/15) * 8)
            orange = 200 + scale
            red = 255
            green = 100 + scale
            draw.point((x, y), fill=(red, green, 50))
    
    # Add darker scale outlines
    for y in range(0, size, 20):
        for x in range(0, size, 20):
            # Draw scale outline
            draw.ellipse([x, y, x+18, y+18], 
                        outline=(180, 80, 0), width=2)
    
    # Add highlights for a glossy look
    for i in range(15):
        x = np.random.randint(0, size)
        y = np.random.randint(0, size)
        radius = np.random.randint(3, 6)
        draw.ellipse([x-radius, y-radius, x+radius, y+radius], 
                    fill=(255, 200, 100))
    
    return img

def main():
    # Create Resources directory if it doesn't exist
    import os
    if not os.path.exists('Resources'):
        os.makedirs('Resources')
    
    # Generate and save textures
    textures = {
        'brick.png': create_brick_texture(),
        'wall.png': create_wall_texture(),
        'help.png': create_help_texture(),
        'snake.png': create_snake_texture()
    }
    
    for filename, img in textures.items():
        path = os.path.join('Resources', filename)
        img.save(path)
        print(f"created {path}")

if __name__ == '__main__':
    main() 