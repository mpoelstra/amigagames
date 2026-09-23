"""Interactive concept compositor using accepted indexed art, no runtime edits."""
from pathlib import Path
import base64,json
ROOT=Path(__file__).resolve().parents[1]
OUT=ROOT/'assets/concept/drowned-sluice-review-v1'
def main():
 OUT.mkdir(parents=True,exist_ok=True)
 images={name:'data:image/png;base64,'+base64.b64encode((ROOT/path).read_bytes()).decode() for name,path in {
 'scene':'assets/concept/drowned-polish-native-v1/preview-1x.png',
 'front':'assets/concept/drowned-polish-native-v1/front-indexed.png',
 'rear':'assets/concept/drowned-polish-native-v1/rear-indexed.png'}.items()}
 template=ROOT/'tools/drowned_sluice_review.html'
 (OUT/'index.html').write_text(template.read_text().replace('/*IMAGES*/{}',json.dumps(images)))
 print(OUT/'index.html')
if __name__=='__main__':main()
