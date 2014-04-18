/****************************************************************************
 Copyright (c) 2013      cocos2d-x.org
 Copyright (c) 2013-2014 Chukong Technologies Inc.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#import "RootViewController.h"
#import "cocos2d.h"
#import "CCEAGLView.h"

@implementation RootViewController

@synthesize photo = _photo;

-(void) takePhoto
{
    UIImagePickerController *picker= [[UIImagePickerController alloc] init];
    picker.delegate = self;
    
    if ([UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypeCamera])
    {
        picker.sourceType = UIImagePickerControllerSourceTypeCamera;
        
    }
    else
    {
        NSLog(@"模拟器无法打开相机");
    }
    [self presentViewController:picker animated:YES completion:nil];
}

//拍照
- (void)imagePickerController:(UIImagePickerController *)picker
didFinishPickingMediaWithInfo:(NSDictionary *)info
{
    NSLog(@"拍照");
    UIImage *originImage = [info objectForKey:UIImagePickerControllerOriginalImage];
    //保存原图到相册
    UIImageWriteToSavedPhotosAlbum(originImage, nil, nil, nil);
    
    //generate a sprite
    _photo = originImage;
    
    
    cocos2d::Image *imf =new cocos2d::Image();
    NSData *imgData = UIImagePNGRepresentation(self.photo);
    NSUInteger len = [imgData length];
    Byte *byteData = (Byte*)malloc(len);
    memcpy(byteData, [imgData bytes], len);
    
    imf->initWithImageData(byteData,imgData.length);
    imf->autorelease();
    
    cocos2d::Texture2D* pTexture = new cocos2d::Texture2D();
    pTexture->initWithImage(imf);
    pTexture->autorelease();
    
    cocos2d::Sprite *sprite = cocos2d::Sprite::createWithTexture(pTexture);
    
    using namespace cocos2d;
    Scene *scene =  Director::getInstance()->getRunningScene();
    auto size = Director::getInstance()->getVisibleSize();
    sprite->setPosition(cocos2d::Point(size.width/2, size.height/2));
    scene->addChild(sprite, 1024);
    
    [picker dismissViewControllerAnimated:false completion:nil];
    
    
}
//取消
- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker {
    NSLog(@"取消");
    
    [picker dismissViewControllerAnimated:false completion:nil];    
    
}




/*
 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        // Custom initialization
    }
    return self;
}
*/

/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
}
*/

/*
// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
}

*/
// Override to allow orientations other than the default portrait orientation.
// This method is deprecated on ios6
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    return UIInterfaceOrientationIsPortrait( interfaceOrientation );
}

// For ios6, use supportedInterfaceOrientations & shouldAutorotate instead
- (NSUInteger) supportedInterfaceOrientations{
#ifdef __IPHONE_6_0
    return UIInterfaceOrientationMaskPortrait;
#endif
}

- (BOOL) shouldAutorotate {
    return YES;
}

- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation {
	[super didRotateFromInterfaceOrientation:fromInterfaceOrientation];

        cocos2d::GLView *glview = cocos2d::Director::getInstance()->getOpenGLView();
        CCEAGLView *eaglview = (CCEAGLView*) glview->getEAGLView();

	CGSize s = CGSizeMake([eaglview getWidth], [eaglview getHeight]);

	cocos2d::Application::getInstance()->applicationScreenSizeChanged((int) s.width, (int) s.height);
}

//fix not hide status on ios7
- (BOOL)prefersStatusBarHidden
{
    return YES;
}

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];

    // Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}


- (void)dealloc {
    [super dealloc];
}


@end
