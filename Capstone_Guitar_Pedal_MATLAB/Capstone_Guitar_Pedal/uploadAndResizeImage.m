% The is the test for the image uploader. The function of this will be to
% upload an image to the images folder from the local computer of the user
% and then resize it to be width of 400px to be included manually in a
% requirement container by the user.

function uploadAndResizeImage()
    [fileName, pathName] = uigetfile({'*.jpg;*.png;*.gif', 'Image Files (*.jpg, *.png, *.gif)'});
    if isequal(fileName, 0)
        disp('User canceled the operation.');
    else
        img = imread(fullfile(pathName, fileName));
        resizedImg = imresize(img, [NaN 300], 'bicubic', 'Antialiasing', true);
        resizedImg = imsharpen(resizedImg, 'Radius', 1, 'Amount', 1.1);
        outPath = fullfile("Images", fileName);
        imwrite(resizedImg, outPath);
        disp(['Saved to: ' outPath]);
    end
end