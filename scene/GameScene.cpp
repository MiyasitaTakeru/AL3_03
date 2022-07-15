#include "GameScene.h"
#include "TextureManager.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "SetMatrix.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ファイル名を指定してテクスチャに読み込む
	textureHandle_ = TextureManager::Load("texture.png");
	//3Dモデルの生成
	model_ = Model::Create();

	//ワールドトランスフォーム
	worldTransform_.Initialize();
	//ビュープロジェクション
	viewProjection_.Initialize();

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
	//軸方向の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

	////X、Y、Z 方向のスケーリングを設定
	//worldTransform_.scale_ = { 2,4,6 };
	////スケーリング行列を宣言
	//Matrix4 matScale;
	////スケーリング倍率を行列に設定
	//matScale = MathUtility::Matrix4Identity();
	//matScale.m[0][0] = worldTransform_.scale_.x;
	//matScale.m[1][1] = worldTransform_.scale_.y;
	//matScale.m[2][2] = worldTransform_.scale_.z;
	////worldTransform_matWorld_ に 単位行列を代入する
	//worldTransform_.matWorld_ = MathUtility::Matrix4Identity();
	////worldTransform_matWorld_ に matScale を掛け算して代入する
	//worldTransform_.matWorld_ *= matScale;
	////行列の転送
	//worldTransform_.TransferMatrix();


	////X、Y、Z 軸周りの回転角を設定
	//worldTransform_.rotation_ = { 20,0,0 };
	////X軸回転行列を宣言
	//Matrix4 matRotX;
	////X軸回転行列の各要素を設定する
	//matRotX = MathUtility::Matrix4Identity();
	//matRotX.m[1][1] = cos(worldTransform_.rotation_.x);
	//matRotX.m[1][2] = sin(worldTransform_.rotation_.x);
	//matRotX.m[2][1] = -sin(worldTransform_.rotation_.x);
	//matRotX.m[2][2] = cos(worldTransform_.rotation_.x);
	////worldTransform_matWorld_ に 単位行列を代入する
	//worldTransform_.matWorld_ = MathUtility::Matrix4Identity();
	////worldTransform_matWorld_ に matRotX を掛け算して代入する
	//worldTransform_.matWorld_ *= matRotX;
	////行列の転送
	//worldTransform_.TransferMatrix();

	////X、Y、Z 軸周りの回転角を設定
	//worldTransform_.rotation_ = { 0,20,0 };
	////Y軸回転行列を宣言
	//Matrix4 matRotY;
	////Y軸回転行列の各要素を設定する
	//matRotY = MathUtility::Matrix4Identity();
	//matRotY.m[0][0] = cos(worldTransform_.rotation_.y);
	//matRotY.m[0][2] = sin(worldTransform_.rotation_.y);
	//matRotY.m[2][0] = -sin(worldTransform_.rotation_.y);
	//matRotY.m[2][2] = cos(worldTransform_.rotation_.y);
	////worldTransform_matWorld_ に 単位行列を代入する
	//worldTransform_.matWorld_ = MathUtility::Matrix4Identity();
	////worldTransform_matWorld_ に matRotY を掛け算して代入する
	//worldTransform_.matWorld_ *= matRotY;
	////行列の転送
	//worldTransform_.TransferMatrix();


	////X、Y、Z 軸周りの回転角を設定
	//worldTransform_.rotation_ = { 0,0,20 };
	////X軸回転行列を宣言
	//Matrix4 matRotZ;
	////X軸回転行列の各要素を設定する
	//matRotZ = MathUtility::Matrix4Identity();
	//matRotZ.m[0][0] = cos(worldTransform_.rotation_.z);
	//matRotZ.m[0][1] = sin(worldTransform_.rotation_.z);
	//matRotZ.m[1][0] = -sin(worldTransform_.rotation_.z);
	//matRotZ.m[1][1] = cos(worldTransform_.rotation_.z);
	////worldTransform_matWorld_ に 単位行列を代入する
	//worldTransform_.matWorld_ = MathUtility::Matrix4Identity();
	////worldTransform_matWorld_ に matRotZ を掛け算して代入する
	//worldTransform_.matWorld_ *= matRotZ;
	////行列の転送
	//worldTransform_.TransferMatrix();
}

void GameScene::Update() {
	//デバッグカメラの更新
	debugCamera_->Update();
	worldTransform_.translation_.x += 0.1f;
	SetMatrix(worldTransform_);
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	//model_->Draw(worldTransform_,viewProjection_, textureHandle_);
	model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	for (float i = 0; i < 10; i++) {
		PrimitiveDrawer::GetInstance()->DrawLine3d({ 0, i ,0 }, { 10, i, 0 }, { 1, 0, 0, 1 });
	}
	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
